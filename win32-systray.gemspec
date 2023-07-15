require_relative "lib/win32/systray/version"

Gem::Specification.new do |spec|
  spec.name = "win32-systray"
  spec.version = Win32::Systray::VERSION
  spec.authors = ["Kimihiko Masamoto"]
  spec.email = ["kmasamoto1980@gmail.com"]

  spec.summary = %q{easy to make system tray application for windows}
  spec.description = %q{light weight, forcus for windows}
  spec.homepage = "https://github.com/kmasa/win32-systray"
  spec.required_ruby_version = Gem::Requirement.new(">= 2.7.0")

  spec.metadata["allowed_push_host"] = "https://github.com/kmasa/win32-systray"

  spec.metadata["homepage_uri"] = spec.homepage
  spec.metadata["source_code_uri"] = "https://github.com/kmasa/win32-systray"
  spec.metadata["changelog_uri"] = "https://github.com/kmasa/win32-systray/blob/main/CHANGELOG.md"

  # Specify which files should be added to the gem when it is released.
  # The `git ls-files -z` loads the files in the RubyGem that have been added into git.
  spec.files = Dir.chdir(File.expand_path("..", __FILE__)) do
    `git ls-files -z`.split("\x0").reject { |f| f.match(%r{^(test|spec|features)/}) }
  end
  spec.bindir = "exe"
  spec.executables = spec.files.grep(%r{^exe/}) { |f| File.basename(f) }
  spec.require_paths = ["lib"]
end
