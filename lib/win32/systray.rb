require "win32/systray/version"

require "fiddle/import"

module Win32
  module ApiSystray
    extend Fiddle::Importer
    dlload File.join(__dir__, "systray.dll")

    extern "void Systray(const wchar_t* tip, const wchar_t* icon_path, void*);"
    extern "void Quit();"
    extern "void* MenuPopup(const wchar_t* text, void void*);"
    extern "void* MenuItem(const wchar_t* text, void void*);"
    extern "void* MenuSeparator();"
  end

  class Systray
    attr_accessor :tip, :icon
    def Systray.run!(&b)
      t = Systray.new(&b)
      t.run!
    end

    def initialize(&b)
      @menu_stack = []
      @menu_root = nil
      b.call(self)
    end

    def run!()
      ApiSystray.Systray(@tip&.encode("UTF-16LE"), @icon&.encode("UTF-16LE"), @block)
    end

    def menu(&b)
      @block = ApiSystray.bind("void Action()", &b)
    end

    def popup(text, &block)
      b = ApiSystray.bind("void Action()", &block)
      ApiSystray.MenuPopup(text.encode("UTF-16LE"), b)
    end

    def item(text, &block)
      b = ApiSystray.bind("void Action()", &block)
      ApiSystray.MenuItem(text.encode("UTF-16LE"), b)
    end

    def separator
      ApiSystray.MenuSeparator()
    end

    def quit
      ApiSystray.Quit()
    end
  end
end
