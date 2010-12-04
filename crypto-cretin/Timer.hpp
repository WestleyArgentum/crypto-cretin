#pragma once

class Timer
{
public:
    /// Auto starts the timer when you create the Timer object.
  Timer( void );

    /// Start the timer to start keeping track since the time of this call
  void Start( void );
    /// Stop the timer, getting the TimeElapsed of a stopped timer will give you the time
    ///  time elapsed between start and stop.
  void Stop( void );

    /// Pause the timer, thus pausing the timer from keeping track of how much time has
    ///  been elapsed since Start()
  void Pause( void );
    ///  Resume the timer, picking up where the timer left off when the timer was paused.
  void Resume( void );

    /// Check to see how much time has been elapsed since the timer has been started.
  const double& TimeElapsed( void );

    /// Check to see if the timer is currently paused.
  bool IsPaused( void ) const { return paused_; }

private:
    /// Starts the timer, making sure that the timers internal data is set properly
  void TrueStart( void );

    /// Resets the timer to be in the starting state again.
  void ResetTimer( void );

    /// Check to see how many seconds have passed
  double SecondsPassed( void ) const;

    /// Updates the time, queries system.
  void UpdateTime( void );

private:
  struct TimerStruct
  {
    LARGE_INTEGER start_;     ///< Holds time information about the start of our frame.
    LARGE_INTEGER end_;       ///< Holds the time information about when we, 

      /// Holds the frequency given by the OS that will help us calculate the current time.
    LARGE_INTEGER frequency_;
  };  // TimerStruct

private:
  TimerStruct data_;      ///< Time information, needed to collect time from the OS.

  double elapsed_;        ///< Time elapsed while the timer is on, running (milliseconds)
  double previous_;       ///< Stores the elapsed time while the timer is paused.
  bool paused_;           ///< Flag that states if our Timer is in a "Paused" state.
  bool done_;             ///< Specifies if the timer is done. ( End() is called )
};  // Timer

