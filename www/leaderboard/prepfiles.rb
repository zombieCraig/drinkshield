#!/usr/bin/ruby
# Simple Ruby script to prepare the files from the DrinkDuino game
# for the web.  Basically just works with the big BMP files that
# the game creates and makes them smaller

# (cc) BY-SA Craig Smith <craig@gfxhax.com>

# Requires librmagick-ruby, libzip-ruby

require 'zip/zip'
require 'RMagick'
include Magick
include Zip

require 'ftools'

ddpath = File.expand_path("~/.drinkduino")
picpath = File.join(ddpath, "pics")

player = Hash.new
newfiles = Array.new

Dir.foreach(picpath) { |pic|
	if pic =~ /(.*)_(\d+)\.bmp$/i then
		name = $1
		num = $2
		puts pic
		if num == "0" then
			profilepic = ImageList.new(File.join(picpath, pic))
			profilethumb = profilepic.scale(0.25)
			profilethumb.write(File.join(picpath, "#{name}_thumb.jpg"))
			newfiles << File.join(picpath, "#{name}_thumb.jpg")
		end
		player[name] = Array.new if not player[name]
		player[name].push( File.join(picpath, pic))
	end
}

puts "Generating Animated Gifs"
player.each { |name,pics|
  puts name
  anim = ImageList.new
  pics.each { |pic| anim.read(pic) }
  anim.each { |a| a.scale!(0.5) }
  anim.delay = 125
  anim.write(File.join(picpath,"#{name}.gif"))
  newfiles << File.join(picpath, "#{name}.gif")
}

if File.exists?(File.join(ddpath, "leaderboard.zip")) then
	File.delete(File.join(ddpath, "leaderboard.zip")) 
end

puts "Archiving to #{File.join(ddpath, "leaderboard.zip")}..."
ZipFile.open(File.join(ddpath, "leaderboard.zip"), Zip::ZipFile::CREATE) { |ar|
  ar.add("drinkScorez", File.join(ddpath, "drinkScorez"))
  ar.mkdir("pics")
  newfiles.each { |pic| ar.add(File.join("pics",File.basename( pic)), pic) }
  ar.close
}

puts "Cleaining up"
newfiles.each { |pic| File.delete(pic) }

