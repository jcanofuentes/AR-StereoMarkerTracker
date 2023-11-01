#ifndef _MINI_DUMPER_
#define _MINI_DUMPER_

/**
 * This class generates minidumps when the application crashes due to an unhandled exception.
 */
class MiniDumper
{
public:

  /// Core dumps types
  enum DUMP_TYPE {
    CALL_STACK_DUMP,  ///< Dump with call stack data.
    FULL_MEMORY_DUMP  ///< Dump with the process' full memory.
  };

  /**
   * Default constructor. Register the object as an unhandled exception filter.
   * @param type The desired type of dump.
   */ 
  MiniDumper(DUMP_TYPE type);
};

#endif
