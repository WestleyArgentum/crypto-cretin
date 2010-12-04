//=======================================================\\

/*!

      @file        Components.cpp

      @author      Robert Onulak
 
      @brief
          Timer object. Query the time elapsed between the start and end of the timer.
           Supports pausing and resuming time.

    Copyright © 2009 DigiPen (USA) Corporation, All Rights Reserved
    Reproduction or disclosure of this file or its contents without 
    the prior written consent of DigiPen Institute of Technology is prohibited.

*/

//========================================================\\

#include "Timer.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// Timer Public Methods


//**************************************************************************************************
Timer::Timer( void )
  : elapsed_(0.0)
  , previous_(0.0)
  , paused_(false)
  , done_(false)
{
    // Initialize our timers internal frequency
  QueryPerformanceFrequency( &data_.frequency_ );

    // Implicitly start our timer
  TrueStart();
}


//**************************************************************************************************
void Timer::Start( void )
{
    // Resets our timer.
  ResetTimer();

    // Then start up our timer again.
  TrueStart();
}


//**************************************************************************************************
void Timer::Stop( void )
{
  if ( !done_ )
  {
    // Stop our timer.
    QueryPerformanceCounter( &data_.end_ );

    done_   = true;
    paused_ = false;    // Remove our timers paused state.
  }
}


//**************************************************************************************************
void Timer::Pause( void )
{
    // If our timer is currently done... (aka, not in a running state...)
  if ( done_ )
  {
      // Don't do anything since our timer hasn't been started yet.
    return;
  }

    // If our timer is not in our paused state yet...
  if ( !paused_ )
  {
    UpdateTime();
    previous_ += elapsed_;

      // Then PAUSE our timer!!
    paused_ = true;
  }
}


//**************************************************************************************************
void Timer::Resume( void )
{
    // If our timer isn't in a paused state OR is done...
  if ( ( !paused_ ) || done_ )
  {
      // Perform a NOP operation and just return.
    return;
  }

    // Re-"start" our timer
  TrueStart();
}


//**************************************************************************************************
const double& Timer::TimeElapsed( void )
{
    // While our timer is pause, skip updating the time since currently our timer is on "hold"
  if ( !paused_ )
  {
    UpdateTime();

      // Otherwise update the current time elapsed from the seconds passed since.
    elapsed_ = previous_ + SecondsPassed();
  }

  return elapsed_;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// Timer Private Methods


//**************************************************************************************************
void Timer::ResetTimer( void )
{
  elapsed_    = 0.0;
  previous_   = 0.0;
  paused_     = false;
  done_       = false;
}


//**************************************************************************************************
void Timer::TrueStart( void )
{
    // Since we are starting our timer, we shouldn't be in a paused state or in a done state.
  paused_ = false;
  done_   = false;

    // Start our timer.
  QueryPerformanceCounter( &data_.start_ );
}


//**************************************************************************************************
double Timer::SecondsPassed( void ) const
{
    // Calculate how many seconds passed since we last checked out time. (milliseconds)
  return (data_.end_.QuadPart - data_.start_.QuadPart)
    / static_cast<double>(data_.frequency_.QuadPart);
}


//**************************************************************************************************
void Timer::UpdateTime( void )
{
  QueryPerformanceCounter( &data_.end_ );
}

