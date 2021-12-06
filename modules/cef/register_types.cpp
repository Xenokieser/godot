/*************************************************************************/
/*  register_types.cpp                                                   */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2021 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2021 Godot Engine contributors (cf. AUTHORS.md).   */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "register_types.h"

#include "cef.h"
#include "cef_canvas_item.h"
#include "cef_control.h"

#include <iostream>

#include <include/cef_app.h>
#include <include/cef_client.h>
#include <include/cef_command_line.h>

#include "webview/shared/browser/main_message_loop_external_pump.h"
#include "webview/shared/browser/main_message_loop_std.h"

#include "webview/shared/browser/client_app_browser.h"
#include "webview/shared/common/client_app_other.h"
#include "webview/shared/common/client_switches.h"
#include "webview/shared/renderer/client_app_renderer.h"

#include "webview/main_context_impl.h"


//godot dependencies 
#include "platform/windows/os_windows.h"

Thread cefInitThread;


static void _cef_init_thread_func(void *p_userdata) {

	//// INIT CEF PROCESSes
	// Enable High-DPI support on Windows 7 or newer.
	CefEnableHighDPISupport();

	void* sandbox_info = nullptr;
	/************************************************************************************/
	/************************************************************************************/

	//CefMainArgs args(((OS_Windows *)OS::get_singleton())->get_hinstance());
	CefMainArgs args(::GetModuleHandle(NULL));

	// Parse command-line arguments.
	CefRefPtr<CefCommandLine> command_line = CefCommandLine::CreateCommandLine();
	command_line->InitFromString(::GetCommandLineW());

	// Create a ClientApp of the correct type.
	CefRefPtr<CefApp> app = new client::ClientAppRenderer();
	//CefRefPtr<CefApp> app = new client::ClientAppBrowser();
	//CefRefPtr<CefApp> app = new client::ClientAppOther();

	// Execute the secondary process, if any.
	int exit_code = CefExecuteProcess(args, app, nullptr);
	if (exit_code >= 0) { 
		 throw std::invalid_argument( "CefMainArgs invalid" );
	}
	// Create the main context object.
	auto context = std::make_unique<client::MainContextImpl>(command_line, true);

	CefSettings settings;

	// Applications should specify a unique GUID here to enable trusted downloads.
	CefString(&settings.application_client_id_for_file_scanning).FromString("9A8DE24D-B822-4C6C-8259-5A848FEA1E68");

	// Populate the settings based on command line arguments.
	context->PopulateSettings(&settings);

	settings.chrome_runtime = true;
	//IMPORTANT windowless mode to avoid the creation of an external window
	settings.windowless_rendering_enabled = true;
	settings.command_line_args_disabled = true;
	//settings.multi_threaded_message_loop = true;
	settings.no_sandbox = true;



	// Create the main message loop object.
	std::unique_ptr<client::MainMessageLoop> message_loop;
	//if (settings.multi_threaded_message_loop)
	//	message_loop.reset(new client::MainMessageLoopMultithreadedWin);
	//else
	if (settings.external_message_pump)
		message_loop = client::MainMessageLoopExternalPump::Create();
	else
		message_loop.reset(new client::MainMessageLoopStd);



	// Create the main message loop object.
	//std::unique_ptr<client::MainMessageLoop> message_loop;
	//message_loop.reset(new MainMessageLoopMultithreadedWin);

	// Initialize CEF.
	context->Initialize(args, settings, app, sandbox_info);
	auto window_config = std::make_unique<client::RootWindowConfig>();
	window_config->always_on_top = command_line->HasSwitch(client::switches::kAlwaysOnTop);
	window_config->with_controls = !command_line->HasSwitch(client::switches::kHideControls);
	window_config->with_osr = settings.windowless_rendering_enabled ? true : false;

	// Create the first window.
	context->GetRootWindowManager()->CreateRootWindow(std::move(window_config));

	/*
	bool result = CefInitialize(args, settings, app, nullptr);
	if (!result) {
		throw std::invalid_argument( "CefInitialize invalid" );
	}
	*/

	//CefRunMessageLoop();
	// Run the message loop. This will block until Quit() is called by the
	// RootWindowManager after all windows have been destroyed.
	int result = message_loop->Run();

	// Shut down CEF.
	context->Shutdown();

	//CefShutdown();

	// Release objects in reverse order of creation.
	message_loop.reset();
	context.reset();
}


static void _cef_init_alt_thread_func(void* p_userdata) {
	//std::cout << ::getpid() << "::" << ::getppid() << ": " << __FILE__ << ": "
	//	<< __PRETTY_FUNCTION__ << std::endl;
	//CefMainArgs args(argc, argv);

#if defined(OS_WIN)
	CefMainArgs args(::GetModuleHandle(NULL));
#else
	// Provide CEF with command-line arguments.
	CefMainArgs args(argc, argv);
#endif

	int exit_code = CefExecuteProcess(args, nullptr, nullptr);
	if (exit_code >= 0)
	{
		//std::cerr << ::getpid() << "::" << ::getppid() << ": "
		std::cerr << "[CEF_start] CefExecuteProcess(): Chromium sub-process has completed"
			<< std::endl;
		exit(exit_code);
	}
	else if (exit_code == -1)
	{
		// we are here in the father proccess.
		//std::cerr << ::getpid() << "::" << ::getppid() << ": "
		std::cerr << "[CEF_start] CefExecuteProcess(): argv not for Chromium: ignoring!" << std::endl;
	}
	//std::cerr << ::getpid() << "::" << ::getppid() << ": "
	std::cerr << "[CEF_start] CefExecuteProcess(): done" << std::endl;

	// Configure Chromium
	CefSettings settings;
	// TODO CefString(&settings.locales_dir_path) = "cef/linux/lib/locales";
	settings.windowless_rendering_enabled = true;
	//settings.ignore_certificate_errors = true;
#if !defined(CEF_USE_SANDBOX)
	settings.no_sandbox = true;
#endif

	bool result = CefInitialize(args, settings, nullptr, nullptr);
	if (!result)
	{
		//std::cerr << ::getpid() << "::" << ::getppid() << ": "
		std::cerr << "[CEF_start] CefInitialize: failed" << std::endl;
		exit(-2);
	}
	//std::cerr << ::getpid() << "::" << ::getppid() << ": "
	std::cerr << "[CEF_start] CefInitialize: OK" << std::endl;

	CefDoMessageLoopWork();

}


//------------------------------------------------------------------------------
//! \brief Stop Chromium Embedded Framework.
//------------------------------------------------------------------------------
static void CEF_stop()
{
	//std::cerr << ::getpid() << "::" << ::getppid() << ": "
	std::cerr << "[CEF_stop]" << std::endl;
	CefShutdown();
}


void register_cef_types()
{
	if (!cefInitThread.is_started())
		cefInitThread.start(_cef_init_alt_thread_func, nullptr);
	//else
	//	cefInitThread.get_main_id();
	
	// Register render node 
	ClassDB::register_class<Cef>();
	ClassDB::register_class<CefCanvasItem>();
	ClassDB::register_class<CefControl>();
}

void unregister_cef_types()
{
}
