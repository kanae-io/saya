require "open-uri"
require "pathname"
require "fileutils"
require "json"

require "pry-byebug"

class Script
  def initialize
    saya_root = ENV["SAYA_ROOT"]
    raise LoadError, "env SAYA_ROOT must be specified" unless saya_root && !saya_root.empty?

    @project_root = Pathname.new(saya_root)
    @path = @project_root.join("src/html/", script_name)
    if @path.exist?
      puts "#{@path} exists, skipping"
      exit 0
    end

    FileUtils.mkdir_p @path.parent

    @json = JSON.parse! open("http://www.w3.org/html/wg/drafts/html/master/entities.json").read
  end

  def script_name
    "named_character_references_table.cpp"
  end

  def script_begin
    <<-END
#include "saya/html/detail/named_character_references_table.hpp"

namespace saya { namespace html { namespace detail {

    END
  end

  def table_begin(wide_variant)
    type = wide_variant ? "wchar_t" : "char"
    name = wide_variant ? "WIDE_" : ""
    "html_traits<#{type}>::named_character_references_table_type const NAMED_CHARACTER_REFERENCES_#{name}TABLE = {\n"
  end

  def table_end
    "};\n"
  end

  def script_end
    <<-END
}}} // saya
    END
  end

  def run
    File.open(@path, "w") {|io|
      io.write script_begin
      run_impl(false, io)
      run_impl(true, io)
      io.write script_end
    }
  end

  private

  def run_impl(wide_variant, io)
    literal_binder = wide_variant ? "L" : ""

    io.write table_begin(wide_variant)
    @json.each do |k, v|
      code = v["codepoints"].map{|cp| sprintf("\\U%08X", cp.to_i) }.join
      io.write "  {#{literal_binder}\"#{k}\", #{literal_binder}\"#{code}\"},\n"
    end
    io.write table_end
    io.write "\n"
  end
end

Script.new.run

