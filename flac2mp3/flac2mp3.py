############################################################################
#    Copyright (C) 2004 by Martin Runge                                    #
#    martin.runge@web.de                                                   #
#                                                                          #
#    This program is free software; you can redistribute it and#or modify  #
#    it under the terms of the GNU General Public License as published by  #
#    the Free Software Foundation; either version 2 of the License, or     #
#    (at your option) any later version.                                   #
#                                                                          #
#    This program is distributed in the hope that it will be useful,       #
#    but WITHOUT ANY WARRANTY; without even the implied warranty of        #
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         #
#    GNU General Public License for more details.                          #
#                                                                          #
#    You should have received a copy of the GNU General Public License     #
#    along with this program; if not, write to the                         #
#    Free Software Foundation, Inc.,                                       #
#    59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             #
############################################################################
#! /usr/bin/env python

import string, sys, os, fnmatch

from TagLib import *
from optparse import OptionParser



def flac2mp3( flac_filename, mp3_filename):
  # print "flac2mp3 : " , flac_filename, "->", mp3_filename
  
  # get the meta tag from the flac file via taglib (taglib python bindings)
  fileref = FileRef(flac_filename)
  tag = fileref.tag()
  # year = tag.year()
  # artist = tag.artist()
  # genre = tag.genre()
  # album = tag.album()
  # track = tag.track()
  # print "year: %s artist: %s album: %s genre: %s" %(year, artist, album, genre)
  
  command = 'flac --decode --stdout "' + flac_filename + '" | lame -h -b 192' \
            + ' --tt "' + tag.title() + '"' \
            + ' --ta "' + tag.artist() + '"'  \
            + ' --tl "' + tag.album() + '"'  \
            + ' --tn ' + '%s' %(tag.track()) \
            + ' --tc "' + tag.comment() + '"'  \
            + ' --ty ' + '%s' %(tag.year()) \
            + ' --tg "' + tag.genre() + '"'  \
            + ' - "' + mp3_filename + '"'

  
  # print command
  success = os.system(command);

  if success == 0:
    fileref2 = FileRef(mp3_filename)
    fileref2.tag = fileref.tag
    fileref2.save()
  
    
  
  
def m3u_flac2mp3(flac_m3u_filename, mp3_m3u_filename, oldext, newext):
  fd = file(mp3_m3u_filename, 'w')
  for line in file(flac_m3u_filename,'r'):
    fname_wo_ext = os.path.splitext(line)[0]
    fd.write(fname_wo_ext + newext + '\n') 
    

          
          
def testTreeWalk(src_dir, dest_dir):
  for root, dirs, files in os.walk(src_dir):
    subdir = string.split(root,'/', 1) # split off the first dir name
    mp3dir = os.path.join(dest_dir, subdir[1])
    
    
    # lame does not create direcoties, so do it here
    if not os.path.exists(mp3dir):
      os.makedirs(mp3dir)
      
    
    # for all file in the directory 'root'
    for flac_file in files: 
      ext = string.lower(os.path.splitext(flac_file)[1][1:]);
      filename_wo_ext =  string.strip(flac_file[:-len(ext)]);
      if ext == 'flac':
        # flac_filename is filename of the flac file with relative path seen from the calling directory
        flac_filename = os.path.join(root,flac_file);
        # mp3_filename is filename for the mp3 file to creaate with relative path seen from the calling directory
        mp3_filename = os.path.join(mp3dir, filename_wo_ext + "mp3");
        
        # next, call flac2mp3 only, if mp3 file does not already exist, or flac file is newer than existing mp3 file
        if (not os.path.exists(mp3_filename)) or (os.path.getctime(mp3_filename) < os.path.getctime(flac_filename)):
          flac2mp3( flac_filename, mp3_filename);
        else:
          print 'file already converted'
          
        # print "Flac file: ", root, "/", flac_file, " -> ", mp3dir , "/", filename_wo_ext, "mp3"          
      elif ext == 'm3u':
        flac_m3u_filename = os.path.join(root, flac_file);
        mp3_m3u_filename = os.path.join(mp3dir, flac_file);
        m3u_flac2mp3(flac_m3u_filename, mp3_m3u_filename, '.flac', '.mp3');
        # print "m3u file: ", root, "/", flac_file
      else:
        pass
      
      #print "deat-dir: ", mp3dir, " will contain files: ", flac_file, ".", ext
    
    
    

def usage():
  print "try " + sys.argv[0] + " --help";

def main():
  parser = OptionParser()
  parser.add_option("-f", "--flacdir", action="store", type="string", dest="flac_dir", help="the direcotry with the flac file")
  parser.add_option("-m", "--mp3dir", action="store", type="string", dest="mp3_dir", help="the direcotry to store the mp3 files in")
  parser.add_option("-q", "--quality", action="store", type="string", dest="quality", help="select quality")
  
  (options, args) = parser.parse_args()
  
  file_types = [ 'flac', 'shn', 'm3u' ];
  
  # get dir to search for flac files.
  if options.flac_dir:
    print "Flac dir = " + options.flac_dir
    src_dir = options.flac_dir
  else:
    usage();
    exit();
  
  if options.mp3_dir:
    print "mp3 dir = " + options.mp3_dir
    dest_dir = options.mp3_dir
  
  if options.quality:
    print "Quality = " + options.quality
    ogg_qual = options.quality

  
  testTreeWalk(src_dir, dest_dir)    
      
  # findFile(src_dir, file_types);
  

  print 'flac2mp3 done!'




main()

