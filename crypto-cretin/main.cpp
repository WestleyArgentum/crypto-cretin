#include "Window.hpp"
#include "FileDialogs.hpp"
#include "Timer.hpp"

#include "encrypt.hpp"

gmp_randstate_t gRandomState;

mpz_class BrentFactorization(const mpz_class &N)
{
  if(N%2 == 0)
    return mpz_class(2);

  mpz_class y = 0,
            ys = 0,
            c = 0,
            m = 0;
  mpz_urandomm(y.get_mpz_t(), gRandomState, N.get_mpz_t());
  mpz_urandomm(c.get_mpz_t(), gRandomState, N.get_mpz_t());
  mpz_urandomm(m.get_mpz_t(), gRandomState, N.get_mpz_t());

  mpz_class g = 1,
            r = 1,
            q = 1,
            lMin = N + 1, //bad (impossible) minimum value to start off with
            lAbs, x, k;

  while(g == 1)
  {
    x = y;

    for(mpz_class i = 0; i < r; ++i)
    {
      y = ((y*y)%N + c)%N;
    }

    k = 0;

    while((k < r) && (g == 1))
    {
      ys = y;

      lMin = m < r-k ? m : r-k;
      for(mpz_class i = 0; i < lMin; ++i)
      {
        y = ((y*y)%N + c)%N;

        mpz_abs(lAbs.get_mpz_t(), mpz_class(x-y).get_mpz_t());
        q = (q*lAbs)%N;
      }

      mpz_gcd(g.get_mpz_t(), q.get_mpz_t(), N.get_mpz_t());
      k = k + m;
    }

    r = r*2;
  }

  if(g==N)
  {
    while(true)
    {
      ys = ((ys*ys)%N + c)%N;

      mpz_abs(lAbs.get_mpz_t(), mpz_class(x-ys).get_mpz_t());
      mpz_gcd(g.get_mpz_t(), lAbs.get_mpz_t(), N.get_mpz_t());

      if(g > 1)
        break;
    }
  }

  return g;
}


enum ButtonIDs
{
  BID_OpenFile = 100
};  // enum ButtonIDs


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
int CALLBACK WinMain(HINSTANCE iInstance, HINSTANCE, LPSTR iCommandLine, int iDisplayParameters)
{
    // Randomness initialization:
  gmp_randinit_default(gRandomState);

    // Create debuging console:
  CreateConsole();

  Timer timer;

  std::string N = "15641574130333";
  std::string p = "3954889", 
              q = "3954997";

    // Create our window's application:
  Window *main_window = Window::GetInstance();
  main_window->Create( "Crypto-cretin", WinProc, iDisplayParameters, "Crypto-cretin", 800, 600 );

  OpenFileDialog dialog_box( main_window->GetHwnd() );

  dialog_box.SetRelativeDir( "." );
  timer.Start();
  if ( dialog_box.OpenFile() )
  {
    printf( "Opened file: %s\n", dialog_box.GetFileName() );
  }
  timer.Stop();
  printf( "You took %lf seconds to find the file you were looking for!\n", timer.TimeElapsed() );

  std::ifstream in("test.txt", std::ios::binary | std::ios::in);
  std::ofstream out("test_enc.txt");

  Encrypt(out, in, mpz_class(N).get_mpz_t(), mpz_class("5").get_mpz_t());

  std::ifstream in2("test_enc.txt", std::ios::in);
  std::ofstream out2("test_dec.txt", std::ios::binary | std::ios::out);

  Decrypt(out2, in2, mpz_class(N).get_mpz_t(), mpz_class("5").get_mpz_t());

    // Run the application, while the window is running:
  while ( main_window->Run() )
  {
  }

    // Deinit random:
  gmp_randclear(gRandomState);

  return main_window->ReturnCode();
}