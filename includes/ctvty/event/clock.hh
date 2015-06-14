#ifndef Clock_hh__
# define Clock_hh__

#include <queue>
#include <list>
#include <chrono>
#include "delayed.hh"
#include "broadcast.hh"

namespace ctvty {
  namespace event {
    /*
     * The Clock guiding the programm
     *  Each Frame a (Update, FixedUpdate, Render) Events will be dispatched
     *  Calling Start, will launch the the programm
     *  Calling End, will stop the dispatching at the end of the current frame and quit the clock
     */
    class Clock {
    private:
      std::list<BroadCast>		broadcasts;
      BroadCast				FixedUpdate;
      BroadCast				Update;
      BroadCast				OnGui;
      BroadCast				Render;
    public:
      template<typename ... broadcast_construction> inline
      void			AddBroadCast(broadcast_construction&& ... params)
      {broadcasts.emplace_back(params...);}
      void			RemoveTarget(GameObject*);

    private:
      std::chrono::milliseconds		frame_length;
      std::chrono::duration<double>	fixedDeltaTime;
      bool				dispatching;
    private:
      double						lastFrameRatio;
      double						maxFrameSeconds;

    private:
      bool						end;

    private:
      std::list<DelayedAction*>				delayedActions;

    public:
      /* Only the Delayed Action will be allowed to call this methods */
      void						AddDelayedAction(DelayedAction*);
      void						RemoveDelayedAction(DelayedAction*);

    private:
      Clock();

    public:
      static Clock&					GetClock();

    public:
      void						SetMaxFramePerSeconds(short);
      float						GetFixedDeltaTime();

      /*
       * if the last frame has been longer than the MaxFrameDuration, the value will be > 1
       */
    public:
      double						LastFrameRatio();

    public:
      void						Start();
      void						End();
    };
  };
};

#endif
