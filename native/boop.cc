#include <v8/v8.h>

#define USE_X11
#include <node/node.h>

#include <iostream>

#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>

#include "atom/browser/atom_browser_client.h"
#include "atom/browser/native_window.h"
#include "atom/browser/window_list.h"

#include "atom/browser/native_window_gtk.h"

#include "atom/browser/atom_browser_main_parts.h"
#include "atom/browser/browser.h"
#include "atom/common/api/atom_bindings.h"
#include "atom/common/node_bindings.h"
#include "atom/common/node_includes.h"

#include "atom/app/atom_library_main.h"

#include "atom/app/atom_main_delegate.h"
#include "content/public/app/content_main.h"

#include "native_mate/dictionary.h"
#include "native_mate/converter.h"
#include "native_mate/wrappable.h"
#include "native_mate/function_template.h"
#include "native_mate/handle.h"

#include "atom/common/native_mate_converters/v8_value_converter.h"
#include "atom/common/native_mate_converters/function_converter.h"
#include "atom/common/native_mate_converters/string16_converter.h"

#include "base/callback.h"
#include "base/callback_forward.h"
#include "base/strings/string16.h"

#include "v8/include/v8.h"

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gtk/gtkwidget.h>
#include <gdk/gdkx.h>


#include "ghbcompositor.h"

SDL_Surface *screen, *hello;

int sdl(void *dummy) {
  SDL_BlitSurface( hello, NULL, screen, NULL );
  SDL_Flip( screen );
  return 0;
  SDL_Init( SDL_INIT_EVERYTHING ); //Set up screen
  SDL_Surface* screen = SDL_SetVideoMode( 640, 480, 32, SDL_SWSURFACE);
  SDL_Surface* hello = SDL_LoadBMP( "lolcats.bmp" );
  SDL_BlitSurface( hello, NULL, screen, NULL );
  SDL_Flip( screen );
  SDL_Delay( 30000 );
  SDL_FreeSurface( screen);
  SDL_Quit();

  return 0;
}

static void Hello(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = v8::Isolate::GetCurrent();
  v8::HandleScope scope(isolate);

 
  std::cout << "NULL = " << NULL << std::endl; 

  // Focus on the first visible window.
  atom::WindowList *list = atom::WindowList::GetInstance();
  size_t size = list->size();
  for (atom::WindowList::iterator iter = list->begin();
       iter != list->end();
       ++iter) {
    atom::NativeWindowGtk *win_atom_gtk =
      (atom::NativeWindowGtk*)*iter;

    GtkWindow *win_ = win_atom_gtk->GetGtkWindow();
    GtkWidget *vbox_ = NULL;
    GtkWidget *native_view = NULL;

    if (win_ != NULL) {
      std::cout << "RECOVER! i=" << std::endl;
      vbox_ = (GtkWidget*)gtk_bin_get_child(GTK_BIN(win_));
    }

    if (vbox_ != NULL) {
      std::cout << "RECOVER! 2" << std::endl;
      GList *children = gtk_container_get_children(GTK_CONTAINER(vbox_));
      native_view = GTK_WIDGET(g_list_first(children)->data);
    }

    std::cout << "WINDOW: #?/" << size
        << " --> " << list
        << " --> " << win_
        << " + " << vbox_
        << " + " << native_view
        << std::endl;

    if (win_ != NULL && vbox_ != NULL) {
      std::cout << "FOUND!!" << std::endl;

      vbox_ = GTK_WIDGET(g_object_ref(vbox_));
      gtk_container_remove(GTK_CONTAINER(win_), vbox_);

      GtkWidget *blender = ghb_compositor_new();
      gtk_container_add(GTK_CONTAINER(win_), GTK_WIDGET(blender));

      GtkWidget *canvas = gtk_event_box_new();
      ghb_compositor_zlist_insert(GHB_COMPOSITOR(blender), canvas, 1, 1);
      ghb_compositor_zlist_insert(GHB_COMPOSITOR(blender), vbox_, 2, 0.6);
	
      gtk_widget_set_size_request(canvas, 640, 480);
      gtk_widget_realize(canvas);
      gtk_widget_show(canvas);

      gtk_widget_set_size_request(GTK_WIDGET(blender), 640, 480);
      gtk_widget_realize(GTK_WIDGET(blender));
      gtk_widget_show(GTK_WIDGET(blender));

	    GdkColor bg = {0, 0, 0, 0};
	    gtk_widget_modify_bg(canvas, GTK_STATE_NORMAL, &bg);
	    //gtk_widget_modify_bg(GTK_WIDGET(vbox_), GTK_STATE_NORMAL, &bg2);
	    //gtk_widget_modify_bg(GTK_WIDGET(vbox_), GTK_STATE_NORMAL, NULL);

			while (gtk_events_pending())
				gtk_main_iteration_do(FALSE);
        
			char SDL_windowhack[128];
			sprintf(SDL_windowhack, "SDL_WINDOWID=%u", (unsigned int)GDK_WINDOW_XID(gtk_widget_get_window(canvas)));
			SDL_putenv(SDL_windowhack);
        
			// init SDL video
      SDL_Init( SDL_INIT_EVERYTHING ); //Set up screen
			if (SDL_WasInit(SDL_INIT_VIDEO)) {
				SDL_QuitSubSystem(SDL_INIT_VIDEO);
      }

			if ( SDL_InitSubSystem(SDL_INIT_VIDEO) < 0 )
			{
				fprintf(stderr, "Couldn't init SDL video: %s\n", SDL_GetError());
				gtk_main_quit();
			} else {
        fprintf(stderr, "Initialized SDL video on %s\n", SDL_windowhack);
      }

      screen = SDL_SetVideoMode( 640, 480, 32, SDL_SWSURFACE);
      hello = SDL_LoadBMP( "lolcats.bmp" );
      sdl(NULL);
  
    }
  }

  args.GetReturnValue().Set(v8::String::New("world"));
}


static void SDL_Open(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = v8::Isolate::GetCurrent();
  v8::HandleScope scope(isolate);

  std::cout << "IN SDL_OPen" << std::endl;

  SDL_CreateThread(sdl, NULL);

  args.GetReturnValue().Set(v8::String::New("SDL"));
}

void init(v8::Handle<v8::Object> exports) {
  NODE_SET_METHOD(exports, "hello", Hello);
  NODE_SET_METHOD(exports, "sdlOpen", SDL_Open);
}

NODE_MODULE(boop, init)

class BoopBrowserMainParts : public atom::AtomBrowserMainParts {
 protected:
  virtual void PostEarlyInitialization() OVERRIDE;
};

void BoopBrowserMainParts::PostEarlyInitialization() {
  atom::AtomBrowserMainParts::PostEarlyInitialization();
  
  v8::Isolate *isolate = v8::Isolate::GetCurrent();
  v8::Locker locker(isolate);
  v8::HandleScope handle_scope(isolate);


  v8::Handle<v8::Object> glob = atom::global_env->process_object();
  
  v8::Local<v8::Object> exports = v8::Object::New();
  boop_module.register_func(exports, v8::Undefined());

  v8::Handle<v8::Value> foo = v8::String::New("boop");
  glob->Set(foo, exports);
}

class BoopBrowserClient : public atom::AtomBrowserClient {
 private:
  virtual brightray::BrowserMainParts* OverrideCreateBrowserMainParts(
      const content::MainFunctionParams&) OVERRIDE;

};

brightray::BrowserMainParts* BoopBrowserClient::OverrideCreateBrowserMainParts(
    const content::MainFunctionParams&) {
  brightray::BrowserMainParts* po =(brightray::BrowserMainParts*) new BoopBrowserMainParts();
  return po;
}

class BoopMainDelegate : public atom::AtomMainDelegate {
  private:
    content::ContentBrowserClient* CreateContentBrowserClient() OVERRIDE;
  scoped_ptr<content::ContentBrowserClient> browser_client_;
};

content::ContentBrowserClient* BoopMainDelegate::CreateContentBrowserClient() {
  browser_client_.reset(new BoopBrowserClient);
  return browser_client_.get();
}

int main(int argc, const char** argv) {
  BoopMainDelegate delegate;
  return content::ContentMain(argc, argv, &delegate);
}

