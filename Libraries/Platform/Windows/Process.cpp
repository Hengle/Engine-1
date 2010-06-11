#include "Platform/Process.h"
#include "Platform/Assert.h"
#include "Platform/Error.h"

#include "Windows.h"

#include <sstream>

int Platform::Execute( const std::string& command, bool showWindow, bool block )
{
  DWORD result = 0;

  STARTUPINFO si;
  memset( &si, 0, sizeof(si) );

  PROCESS_INFORMATION pi;
  memset( &pi, 0, sizeof( pi ) );

  // Start the child process.
  if( !CreateProcess(
    NULL,                                                 // No module name (use command line)
    (LPTSTR) command.c_str(),                            // Command line
    NULL,                                                 // Process handle not inheritable
    NULL,                                                 // Thread handle not inheritable
    FALSE,                                                // Set handle inheritance to FALSE
    showWindow ? CREATE_NEW_CONSOLE : CREATE_NO_WINDOW,  // Creation flags
    NULL,                                                 // Use parent's environment block
    NULL,                                                 // Use parent's starting directory
    &si,                                                  // Pointer to STARTUPINFO structure
    &pi ) )                                               // Pointer to PROCESS_INFORMATION structure
  {
    return -1;
  }

  if (block)
  {
    DWORD waitResult = ::WaitForSingleObject( pi.hProcess, INFINITE );
    NOC_ASSERT( waitResult != WAIT_FAILED );

    BOOL codeResult = ::GetExitCodeProcess( pi.hProcess, &result );
    NOC_ASSERT( codeResult );
  }

  ::CloseHandle( pi.hProcess );
  ::CloseHandle( pi.hThread );

  return result;
}

int Platform::Execute( const std::string& command, std::string& output, bool showWindow )
{
  HANDLE hReadPipe;
  HANDLE hWritePipe;
  SECURITY_ATTRIBUTES sa;
  sa.nLength              = sizeof(sa);
  sa.lpSecurityDescriptor = NULL;
  sa.bInheritHandle       = TRUE;
  if( !CreatePipe( &hReadPipe, &hWritePipe, &sa, 0 ) )
  {
    return -1;
  }

  STARTUPINFO          si;
  memset( &si, 0, sizeof(si) );
  si.cb          = sizeof(si);
  si.dwFlags     = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
  si.wShowWindow = static_cast<int>( showWindow );
  si.hStdOutput  = hWritePipe;
  si.hStdError   = hWritePipe;      

  PROCESS_INFORMATION  pi;
  memset( &pi, 0, sizeof( pi ) );

  if( !CreateProcess(
    NULL,                                                 // filename
    (LPSTR) command.c_str(),                             // command line for child
    NULL,                                                 // process security descriptor
    NULL,                                                 // thread security descriptor
    TRUE,                                                 // inherit handles?
    showWindow ? CREATE_NEW_CONSOLE : CREATE_NO_WINDOW,  // creation flags
    NULL,                                                 // inherited environment address
    NULL,                                                 // startup dir; NULL = start in current
    &si,                                                  // pointer to startup info (input)
    &pi ) )                                               // pointer to process info (output)
  {
    ::CloseHandle( hReadPipe );
    ::CloseHandle( hWritePipe );
    return -1;
  }

  // close the write end of the pipe so the child will terminate it with EOF
  ::CloseHandle( hWritePipe );

  // read from the pipe until EOF condition reached
  char buffer[80];
  unsigned long count;
  std::ostringstream str;
  BOOL success = TRUE;
  do
  {
    while ( success = ReadFile( hReadPipe, buffer, sizeof(buffer), &count, NULL ) )
    {
      if( success )
      {
        str.write( buffer, count );
      }
      else
      {
        if ( ::GetLastError() == ERROR_BROKEN_PIPE )
        {
          break;
        }
        else
        {
          return -1;
        }
      }
    }
  } while( success && count );

  // done reading, close our read pipe
  ::CloseHandle( hReadPipe );

  // copy output string
  output = str.str();

  // get exit code
  DWORD result = 0;
  BOOL codeResult = ::GetExitCodeProcess( pi.hProcess, &result );
  NOC_ASSERT( codeResult );

  // close the process handle
  ::CloseHandle( pi.hProcess );
  ::CloseHandle( pi.hThread );

  return result;
}

std::string Platform::GetProcessString()
{
  HMODULE moduleHandle = GetModuleHandle( NULL );

  char module[ MAX_PATH + 1 ];
  GetModuleFileName( moduleHandle, module, MAX_PATH );

  char file[ MAX_PATH + 1 ];
  _splitpath( module, NULL, NULL, file, NULL );

  std::ostringstream result;
  result << file << "_" << GetCurrentProcessId() << "_" << GetCurrentThreadId();
  return result.str();
}