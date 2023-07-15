require "win32/systray"

Win32::Systray.run! do |sys|
  sys.tip = "pnow"
  sys.icon = File.join(__dir__, "pnow.ico")

  sys.menu do
    sys.popup "テスト1" do
      sys.item "テスト2" do
        puts "hi test2"
      end
    end
    sys.separator
    sys.item "終了" do
      sys.quit
    end
  end
end
