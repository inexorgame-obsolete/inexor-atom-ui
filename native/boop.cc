#include <v8/v8.h>

#define USE_X11
#include <node/node.h>

#include <iostream>

#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>

#include "atom/browser/atom_browser_client.h"
#include "atom/browser/native_window.h"
#include "atom/browser/window_list.h"

#define private public
#include "atom/browser/native_window_gtk.h"
#undef private

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

static void Hello(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = v8::Isolate::GetCurrent();
  v8::HandleScope scope(isolate);

  atom::WindowList *list = atom::WindowList::GetInstance();
  size_t list_s = 4;

  size_t i = 0;
  for (; i < list_s; i++) {
    atom::NativeWindow *win = list->get(i);
    atom::NativeWindowGtk *win_atom_gtk = (atom::NativeWindowGtk*) win;

    GtkWindow *win_ = win_atom_gtk->window_;
    GtkWidget *vbox_ = win_atom_gtk->vbox_;

    std::cout << "WINDOW: #" << i << "/" << list_s
        << " --> " << list
        << " --> " << win
        << " --> " << win_atom_gtk
        << " --> " << win_
        << " + " << vbox_
        << std::endl;

    if (win_ != NULL && vbox_ != NULL) {
      std::cout << "FOUND!!" << std::endl;
      break;
      g_object_ref(vbox_);
      gtk_container_remove(GTK_CONTAINER(win_), vbox_);

      GtkFixed *fixed = (GtkFixed*)gtk_fixed_new();
      gtk_fixed_put(fixed, vbox_, (gint)0, (gint)0);

    }
  }

  args.GetReturnValue().Set(v8::String::New("world"));
}

int sdl(void *dummy) {
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

