SDL 2.0.5 Audio Memory Leak Example
===================================

Mac OSX 10.10.5

[Bug Report Link](https://bugzilla.libsdl.org/show_bug.cgi?id=3649)

This is a [bare-bones program](https://github.com/voidqk/sdl-audio-leak/blob/master/src/main.c) that
opens an audio device, and outputs nothing.  As time goes on, the memory consumption of the program
continues to increase.  Here are random samples of `top` after a bit of waiting:

```
$ top -stats command,time,mem -pid `pgrep sdl`

COMMAND                TIME             MEM
sdl-audio-leak         00:00.16         2384K+
sdl-audio-leak         00:02.94         2892K+
sdl-audio-leak         00:06.02         3452K+
sdl-audio-leak         00:09.02         3996K
sdl-audio-leak         00:12.02         4524K+
sdl-audio-leak         00:15.03         5052K+
```

Once the audio device is closed via `SDL_CloseAudioDevice` (and before `SDL_Quit`), the memory usage
stops growing:

```
sdl-audio-leak         00:15.43         5032K
```

The `top` program doesn't update `sdl-audio-leak` after this point, and it stays at `5032K`.

Tentative Fix
-------------

I've discovered a tentatitive fix, but it isn't very satisfying.

In `SDL_coreaudio.m`, in the function `audioqueue_thread`, change the inner loop from:

```c
    while (!SDL_AtomicGet(&this->hidden->shutdown)) {
        CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0.10, 1);
    }
```

To:

```c
    while (!SDL_AtomicGet(&this->hidden->shutdown)) {
        CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0, 1);
        SDL_Delay(20);
    }
```
