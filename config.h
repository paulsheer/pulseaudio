/* PulseAudio 17.0 — minimal config for a Windows/MinGW x86_64 static build.
 * Server-only daemon compiled into libpulseaudio-embedded.a (part of vcxsrv).
 *
 * All external/optional dependencies are disabled: no dbus, alsa, X11,
 * systemd, avahi, bluez, jack, lirc, fftw, soxr, speex, webrtc, iconv, ORC.
 * MinGW-w64's CRT is missing the POSIX networking/mman/grp/pwd headers, so the
 * matching HAVE_* macros are deliberately left undefined — PulseAudio's own
 * wrapper headers (pulsecore/socket.h, poll.h, arpa-inet.h) plus the #ifdef
 * HAVE_* guards in the sources handle the Windows fallbacks.
 *
 * regex.h is provided by the vendored libregex (glibc-compatible regex), not by
 * the toolchain — see Makefile.mingw64's libregex section. langinfo.h is
 * deliberately not used (see HAVE_LANGINFO_H note below). */
#ifndef PULSEAUDIO_CONFIG_H
#define PULSEAUDIO_CONFIG_H

/* Package identity (sf_version_string()-style concatenation in sndfile.c is
 * not used here; pa_get_library_version() uses PA_MAJOR/PA_MINOR). */
#define PACKAGE "pulseaudio"
#define PACKAGE_NAME "pulseaudio"
#define PACKAGE_VERSION "17.0"
#define VERSION "17.0"
#define PA_MAJOR 17
#define PA_MINOR 0
#define PA_API_VERSION 12
#define PA_PROTOCOL_VERSION 35
#define PA_CFLAGS "-std=gnu11 -DHAVE_CONFIG_H -D_GNU_SOURCE -DEMBEDDED"
#define PA_SOEXT ".dll"
#define GETTEXT_PACKAGE "pulseaudio"
/* ENABLE_NLS deliberately left undefined: gettext/libintl is not part of the
 * minimal clang-mingw toolchain, so pulsecore/i18n.h falls back to its no-op
 * _()/N_() macros and never includes <libintl.h>. */

/* Target: Windows x86_64, little-endian, GCC/Clang. */
#define OS_IS_WIN32 1
#define HAVE_WINDOWS_H 1
#define HAVE_WAVEOUT 1
#define HAVE_WINSOCK2_H 1
#define HAVE_WS2TCPIP_H 1
#define WIN32_LEAN_AND_MEAN 1

/* MinGW does not provide gid_t/uid_t (POSIX types); substitute int. */
#define gid_t int
#define uid_t int
#define GETGROUPS_T gid_t

/* Headers present in MinGW-w64 (plus libregex's regex.h/langinfo.h). */
#define HAVE_STDINT_H 1
#define HAVE_SCHED_H 1
#define HAVE_LOCALE_H 1
#define HAVE_CPUID_H 1
#define HAVE_STD_BOOL 1
#define HAVE_REGEX_H 1
/* HAVE_LANGINFO_H deliberately left undefined: MinGW has no <langinfo.h>, and
 * the vendored libregex langinfo.h is a gnulib replacement that switches on its
 * own HAVE_LANGINFO_H value (0 for MinGW). pulseaudio's config.h defines that
 * same macro to 1, which forces the header's "native langinfo exists" branch and
 * breaks (nl_item undefined). The only consumer is pa_parse_boolean's localized
 * yes/no lookup, which already handles English y/yes/n/no case-insensitively,
 * so we simply drop it. */
#define HAVE_PTHREAD_PRIO_INHERIT 1

/* C runtime functions present in MinGW-w64. */
#define HAVE_FSTAT 1
#define HAVE_GETADDRINFO 1
#define HAVE_GETTIMEOFDAY 1
#define HAVE_LRINTF 1
#define HAVE_OPEN64 1
#define HAVE_STRTOF 1

/* Feature toggles (matching meson's Windows configuration). */
#define HAVE_FAST_64BIT_OPERATIONS 1
#define HAVE_ATOMIC_BUILTINS 1
#define HAVE_ATOMIC_BUILTINS_MEMORY_MODEL 1
#define HAVE_SIMPLEDB 1
#define USE_SMOOTHER_2 1
#define HAVE_IPV6 1
#define DISABLE_ORC 1

/* Explicitly disabled (meson zeroes these on Windows / without the dep). */
#define HAVE_COREAUDIO 0
#define HAVE_AVAHI 0
#define HAVE_GSETTINGS 0

/* Paths. On Windows these are mostly fallbacks: the daemon resolves its
 * config/runtime dirs relative to the executable (pa_win32_get_toplevel) and
 * ProgramData (pa_win32_get_system_appdata) at runtime, using PA_DEFAULT_CONFIG_DIR
 * only as a sentinel prefix that gets stripped and replaced. */
#define PA_DEFAULT_CONFIG_DIR "/etc/pulse"
#define PA_SYSTEM_RUNTIME_PATH "/var/run/pulse"
#define PA_SYSTEM_CONFIG_PATH "/var/lib/pulse"
#define PA_SYSTEM_STATE_PATH "/var/lib/pulse"
#define PA_MACHINE_ID "/etc/machine-id"
#define PA_MACHINE_ID_FALLBACK "/var/lib/dbus/machine-id"
#define PA_BINARY "pulseaudio"
#define PA_DLSEARCHPATH "/usr/lib/pulse-17.0/modules"
#define PA_SYSTEM_USER "pulse"
#define PA_SYSTEM_GROUP "pulse"
#define PA_ACCESS_GROUP "pulse-access"
#define PA_ALSA_DATA_DIR "/usr/share/pulseaudio/alsa-mixer"
#define DESKTOPFILEDIR "/usr/share/applications"
#define PULSE_LOCALEDIR "/usr/share/locale"

#endif /* PULSEAUDIO_CONFIG_H */
