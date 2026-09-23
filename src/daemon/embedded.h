#ifndef fooembeddedhfoo
#define fooembeddedhfoo

/***
  This file is part of PulseAudio.

  PulseAudio is free software; you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published
  by the Free Software Foundation; either version 2.1 of the License,
  or (at your option) any later version.

  PulseAudio is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with PulseAudio; if not, see <http://www.gnu.org/licenses/>.
***/

#ifdef EMBEDDED

#include <stddef.h>

/* Start the PulseAudio daemon (real_main) on a background thread.
 *
 * Blocks until the daemon has finished initializing and bound its sockets,
 * or until startup fails. Returns 0 on success (the daemon thread keeps
 * running); on failure returns -1 and stores a description of the error in
 * error (a buffer of error_len bytes). */
int start_pulseaudio_thread(int argc, char *argv[], char *error, size_t error_len);

#endif /* EMBEDDED */

#endif
