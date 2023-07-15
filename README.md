# win32-systray
ruby gems, easy to make system tray application for windows


## Installation

Add this line to your application's Gemfile:

```ruby
gem 'win32-systray'
```

And then execute:

    $ bundle install

Or install it yourself as:

    $ gem install win32-systray

## Usage

```ruby
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

```

## Contributing

Bug reports and pull requests are welcome on GitHub at https://github.com/kmasa/win32-systray.

