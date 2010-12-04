#include "Window.hpp"
#include "FileDialogs.hpp"
#include "Timer.hpp"

#include "encrypt.hpp"
#include "BrentJob.hpp"


const char *const EXE_NAME = "crypto-cretin";


enum ButtonIDs
{
  BID_OpenFile = 100
};  // enum ButtonIDs


//**************************************************************************************************
void PrintUsage( const char *exe_name )
{
  printf( "Usage: %s [-E | -D] [ ... ]\n\n", exe_name );
  printf( "-E = Encrypt\n" );
  printf( "Commands:\n" );
  printf( "-i=<file>        : Input file to encrypt\n" );
  printf( "-o=<file>        : Resulting encrypted output filename (.enc).\n"
          "                    Default is to append \".enc\" to the input filename.\n" );
  printf( "-N=<number>      : N to use in the RSA encryption.\n" );
  printf( "-e=<number>      : e to use in the RSA encryption.\n" );
  printf( "-cs=<number>     : Chunk size (default = 5).\n" );
  printf( "-eb=(-1 | 0 | 1) : Endianness, order of bits (default = 0).\n" );
  printf( "-ew=(-1 | 0 | 1) : Endianness, order of words (default = 0).\n" );

  printf( "\n" );
  printf( "-D = Decrypt\n" );
  printf( "Commands:\n" );
  printf( "-i=<file>        : Input file to decrypt (.enc)\n" );
  printf( "-o=<file>        : Name of the decrypted file.\n"
          "                    Default is to strip the \".enc\" extension off the output filename.\n" );
  printf( "-eb=(-1 | 0 | 1) : Endianness, order of bits.\n" );
  printf( "-ew=(-1 | 0 | 1) : Endianness, order of words.\n" );

  printf( "\n\n" );
  printf( "Example:\n" );
  printf( "crypto-cretin -E -i=puppy.png -o=puppy.png.enc -N=15641574130333 -e=221 -cs=5 -eb=1 -ew=1\n" );
  printf( "crypto-cretin -D -i=puppy.png.enc -o=puppy.png -eb=1 -ew=1\n" );
}


//**************************************************************************************************
LRESULT CALLBACK WinProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
  PAINTSTRUCT ps;
  HDC hdc;
  int id;

  std::string str;

  switch ( message )
  {
  case WM_PAINT:
    hdc = BeginPaint( hWnd, &ps );
    EndPaint( hWnd, &ps );
    break;

  case WM_DESTROY:
    PostQuitMessage( 0 );
    break;

  case WM_KEYDOWN:
    switch ( wParam )
    {
    case VK_RETURN:
      break;
    }
    break;

  case WM_COMMAND:
    if ( id = LOWORD( wParam ) )
    {
      switch ( id )
      {
        // Open up the filename given:
      case BID_OpenFile:
        {
        }
        break;
      }
    }
    else
    {
      switch ( wParam )
      {
      case BN_CLICKED:
        std::cout << "Unknown button pressed: #" << GetDlgCtrlID( (HWND)lParam ) << std::endl;
        break;
      }
    }
    break;

  case WM_DROPFILES:
    {
        // Query to see how many files were dropped onto our application window!
      unsigned count = DragQueryFile( (HDROP)wParam, 0xFFFFFFFF, 0, 0 );

        // If at least one was dropped, process only one file to encrypt or decrypt
      if ( count )
      {
        char buffer[MAX_PATH + 1] = {0};

        DragQueryFile( (HDROP)wParam, 0, buffer, sizeof(buffer) - 1 );
        DragFinish( (HDROP)wParam );
      }
    }
    break;
  }

  return DefWindowProc( hWnd, message, wParam, lParam );
}


//**************************************************************************************************
int main_fn( int argc, char* argv[] )
{
    // Not enough command line paramters:
  if ( argc < 1 )
  {
    printf( "Not enough command line parameters specified.\n\n" );

    PrintUsage( EXE_NAME );
    return -1;
  }

    // If we are encrypting our file:
  if ( strcmp( argv[0], "-E" ) == 0 )
  {
    printf( "Encrypting:\n" );

    const char *input_file = NULL;
    std::string output_file;

    const char *N = NULL;
    const char *e = NULL;
    int chunk_size = 5;

    int bit_endianness = 0;
    int word_endianness = 0;

      // Collect all of our command line paramters:
    for ( int i = 1; i < argc; ++i )
    {
        // Read in the input filename:
      if ( strncmp( "-i=", argv[i], 3 ) == 0 )
      {
        input_file = argv[i] + 3;
      }
        // Read in the output filename:
      else if ( strncmp( argv[i], "-o=", 3 ) == 0 )
      {
        output_file = argv[i] + 3;
      }
        // RSA, N value
      else if ( strncmp( argv[i], "-N=", 3 ) == 0 )
      {
        N = argv[i] + 3;
      }
        // RSA, e value
      else if ( strncmp( argv[i], "-e=", 3 ) == 0 )
      {
        e = argv[i] + 3;
      }
        // RSA, chunk size
      else if ( strncmp( argv[i], "-cs=", 4 ) == 0 )
      {
        chunk_size = atoi( argv[i] + 4 );
      }
        // Endianness of bits
      else if ( strncmp( argv[i], "-eb=", 4 ) == 0 )
      {
        bit_endianness = atoi( argv[i] + 4 );
      }
        // Endianness of words
      else if ( strncmp( argv[i], "-ew=", 4 ) == 0 )
      {
        word_endianness = atoi( argv[i] + 4 );
      }
    }

      // If the user hasn't supplied the necessary switches:
    if ( !input_file )
    {
      printf( "Input file hasn't been specified. (-i switch).\n" );
      return -1;
    }
    if ( !N )
    {
      printf( "RSA, N hasn't been specified. (-N switch).\n" );
      return -1;
    }
    if ( !N )
    {
      printf( "RSA, e hasn't been specified. (-e switch).\n" );
      return -1;
    }

      // If the user didn't specify the output file, the output file is the input filename with
      //  and .enc appended onto it.
    if ( output_file.empty() )
    {
      output_file = input_file;
      output_file.append( ".enc" );
    }

      // Debug print the command line arguments:
    printf( "crypto-cretin -E -i=%s -o=%s -N=%s -e=%s -cs=%d -eb=%d -ew=%d\n",
      input_file, output_file.c_str(), N, e, chunk_size, bit_endianness, word_endianness );

    std::ifstream in( input_file, std::ios::binary | std::ios::in );
    if ( !in.is_open() )
    {
      printf( "Input file not found.\n" );
      return -1;
    }

    std::ofstream out( output_file.c_str() );
    if ( !out.is_open() )
    {
      printf( "Output file is write protected.\n" );
      return -1;
    }

      // Encrypt our file with the information given.
    Encrypt( out, in, mpz_class(N).get_mpz_t(), mpz_class(e).get_mpz_t(), chunk_size );

    printf( "File encrypted.\n" );
    return 0;
  }
    // If we are decrypting our file:
  else if ( strcmp( argv[0], "-D" ) == 0 )
  {
    printf( "Decrypting:\n" );

    const char *input_file = NULL;
    std::string output_file;

    int bit_endianness = 0;
    int word_endianness = 0;

      // Collect all of our command line paramters:
    for ( int i = 1; i < argc; ++i )
    {
        // Read in the input filename:
      if ( strncmp( "-i=", argv[i], 3 ) == 0 )
      {
        input_file = argv[i] + 3;
      }
        // Read in the output filename:
      else if ( strncmp( argv[i], "-o=", 3 ) == 0 )
      {
        output_file = argv[i] + 3;
      }
        // Endianness of bits
      else if ( strncmp( argv[i], "-eb=", 4 ) == 0 )
      {
        bit_endianness = atoi( argv[i] + 4 );
      }
        // Endianness of words
      else if ( strncmp( argv[i], "-ew=", 4 ) == 0 )
      {
        word_endianness = atoi( argv[i] + 4 );
      }
    }

      // If the user hasn't supplied the necessary switches:
    if ( !input_file )
    {
      printf( "Input file hasn't been specified. (-i switch).\n" );
      return -1;
    }

      // If the user didn't specify the output file name, strip off the extension of the file
    if ( output_file.empty() )
    {
      Tools::FileInfo lInputFile(input_file);
      if ( ".enc" == lInputFile.Extension() )
      {
        output_file = lInputFile.FileNameOnDisk();
      }
        // Invalid input name:
      else
      {
        printf( "Invalid input filename [%s], looking for the extension .enc\n", input_file );
      }
    }

    printf( "crypto-cretin -D -i=%s -o=%s -eb=%d -ew=%d\n",
      input_file, output_file.c_str(), bit_endianness, word_endianness );

    std::ifstream in( input_file, std::ios::in );
    if ( !in.is_open() )
    {
      printf( "Input file not found.\n" );
      return -1;
    }

    std::ofstream out( output_file.c_str(), std::ios::binary | std::ios::out );
    if ( !out.is_open() )
    {
      printf( "Output file is write protected.\n" );
      return -1;
    }

    Decrypt( out, in );
    printf( "File decrypted.\n" );
    return 0;
  }
    // Unknown first switch:
  else
  {
    printf( "Unknown first switch [%s]\n", argv[1] );
    return -1;
  }

  std::string N = "15641574130333";
  std::string p = "3954889",
              q = "3954997";

  Timer timer;

  BrentManager = new BrentJobManager( mpz_class(N) );

  BrentManager->RunBrentFactorization( 10 );

  //std::string N = "15641574130333";
  //std::string p = "99990001", 
  //            q = "2796203";

  //std::string N = "15641574130333";
  //std::string p = "2147483647", 
  //            q = "2305843009213693951";

  std::cout << "Generating P...";
  mpz_class P = 0, Q = 0, lN = 0;
  mpz_ui_pow_ui(P.get_mpz_t(), 2, 31u);
  --P;
  std::cout << " Done" << std::endl;
  
  std::cout << "Generating Q...";
  mpz_ui_pow_ui(Q.get_mpz_t(), 2, 61u);
  --Q;
  std::cout << " Done" << std::endl;

  std::cout << "Generating N...";
  lN = P*Q;
  std::cout << " Done" << std::endl;

  std::cout << "Beginning factorization...";
  mpz_class lBrent = BrentFactorization(lN);
  std::cout << " Done" << std::endl;

  if(lBrent == P || lBrent == Q)
    std::cout << "P or Q" << std::endl;
  else if(lBrent == lN)
    std::cout << "N" << std::endl;
  else
    std::cout << "Neither" << std::endl;

  std::cin.get();
  return 0;

    // Create the job manager to factor the N.
  BrentManager = new BrentJobManager( mpz_class(N) );
    // Factor the N:
  BrentManager->RunBrentFactorization( 10 );

  std::ifstream in("test.pdf", std::ios::binary | std::ios::in);
  std::ofstream out("test_enc.txt");

  Encrypt(out, in, mpz_class(N).get_mpz_t(), mpz_class("5").get_mpz_t());
  out.close();

  std::ifstream in2("test_enc.txt", std::ios::in);
  std::ofstream out2("test_dec.pdf", std::ios::binary | std::ios::out);

  Decrypt(out2, in2);
}


//**************************************************************************************************
int CALLBACK WinMain(HINSTANCE iInstance, HINSTANCE, LPSTR iCommandLine, int iDisplayParameters)
{
    // Randomness initialization:
  gmp_randinit_default(gRandomState);

    // Create debuging console:
  CreateConsole();

    // Collect the command line parameters as if we were "int main(argc, argv)"
  int argc;
  char **argv = CommandLineToArgvA( iCommandLine, &argc );

    // Call our application function to handle everything else about our program.
  int return_code = main_fn( argc, argv );

    // Deinit random:
  gmp_randclear(gRandomState);

  printf( "Press enter to continue...\n" );
  std::cin.get();
  return return_code;
}

