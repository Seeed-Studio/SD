/*

 SD - a slightly more friendly wrapper for sdfatlib

 This library aims to expose a subset of SD card functionality
 in the form of a higher level "wrapper" object.

 License: GNU General Public License V3
          (Because sdfatlib is licensed with this.)

 (C) Copyright 2010 SparkFun Electronics

 */

#ifndef __SD_H__
#define __SD_H__

#include <WProgram.h>

#include <utility/SdFat.h>
#include <utility/SdFatUtil.h>

#define FILE_READ O_READ
#define FILE_TRUNCATE (O_WRITE | O_CREAT | O_TRUNC)
#define FILE_APPEND (O_WRITE | O_CREAT | O_APPEND)

// for reading directories and listing files
class DirectoryEntry {
 public:
  DirectoryEntry(uint8_t *, uint32_t, bool);
  DirectoryEntry(void);
  operator bool() {
    return _exists;
  }
  char *name(void) {
    return _name;
  }
  uint32_t size(void) {
    return _filesize;
  }
  boolean isDirectory(void) {
    return _isDirectory;
  }

 private:
  char _name[13];       // 8+3 add dot and terminating 0
  uint32_t _filesize;

  boolean _isDirectory;
  boolean _exists;

};

class File : public Stream {
 private:
  char _name[13];
  SdFile _file;
  int _c;

public:
  File(SdFile f, char *);
  File(void);
  ~File(void);

  virtual void write(uint8_t);
  virtual void write(const char *str);
  virtual void write(const uint8_t *buf, size_t size);
  virtual int read();
  virtual int read(void *buf, uint16_t nbyte);
  virtual int peek();
  virtual boolean seekSet(uint32_t x);
  virtual uint32_t size(void);
  virtual int available();
  virtual void flush();
  void close();
  operator bool();
  boolean isDirectory();
  void rewindDirectory();
  File openNextFile(void);
  char *name(void);
};

class SDClass {

 private:

 public:
  // we should expose these since the wrapper is so thin

  // These are required for initialisation and use of sdfatlib
  SdFile root;
  Sd2Card card;
  SdVolume volume;
  
  // This needs to be called to set up the connection to the SD card
  // before other methods are used.
  boolean begin(uint8_t csPin = SD_CHIP_SELECT_PIN);
  
  // Open the specified file/directory with the supplied mode (e.g. read or
  // write, etc). Returns a File object for interacting with the file.
  // Note that currently only one file can be open at a time.
  File open(char *filename, uint8_t mode = FILE_READ);

  // Methods to determine if the requested file path exists.
  boolean exists(char *filepath);

  // Create the requested directory heirarchy--if intermediate directories
  // do not exist they will be created.
  boolean mkdir(char *filepath);
  
  // Delete the file.
  boolean remove(char *filepath);
  
  boolean rmdir(char *filepath);


private:

  // This is used to determine the mode used to open a file
  // it's here because it's the easiest place to pass the 
  // information through the directory walking function. But
  // it's probably not the best place for it.
  // It shouldn't be set directly--it is set via the parameters to `open`.
  int fileOpenMode;
  
  int c;
  
  friend class File;
  friend boolean callback_openPath(SdFile&, char *, boolean, void *); 
};

extern SDClass SD;

#endif