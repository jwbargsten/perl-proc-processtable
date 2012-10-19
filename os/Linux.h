
/* Proc::ProcessTable functions */
void ppt_warn(const char*, ...);
void bless_into_proc(char* , char**, ...);

/* it also gets used by init_static_vars at the way top of the file,
 * I wanted init_static_vars to be at the way top close to the global vars */
static char *read_file(const char *path, const char *extra_path, off_t *len,
	struct obstack *mem_pool);

struct procstat
{
	/* user/group id of the user running it */
	int				uid;
	int				gid;
	/* values scraped from /proc/{$pid}/stat */
	pid_t				pid;
	char				comm[16];					/* limit in kernel, likewise in procps */
	char				state_c;
	int					ppid;
	int					pgrp;
	int					sid;
	int					tty;
	unsigned 			flags;
	unsigned long		minflt, cminflt, majflt, cmajflt;
	unsigned long long	utime, stime;
	long long			cutime, cstime;
	long				priority;
	unsigned long long 	start_time;
	unsigned long		vsize;
	long 				rss;
	unsigned long		wchan;
	/* these are derived from above time values */
	unsigned long long	time, ctime;
	/* from above state_c but fixed up elese where */
	const char		*state;
	/* values scraped from /proc/{$pid}/status */
	int				euid, suid, fuid;
	int				egid, sgid, fgid;
	/* cwd, cmdline & exec files; values allocated at the end of obstacks */
	char			*cwd;
	char			*cmndline;
	char			*exec;
	/* other values */
	char			pctcpu[sizeof("100.00")];	/* precent cpu, without '%' char */
	char			pctmem[sizeof("100.00")];	/* precent memory, without '%' char */
};


enum state
{
	SLEEP,
	WAIT,
	RUN,
	IDLE,
	DEFUNCT,
	STOP,
	UWAIT,
};


/* strings, to make sure they get placed in read only memory,
 * ditto for pointers to them and so we avoid relocations */
static const char strings[] =
{
/* process state */
	"sleep\0" "wait\0" "run\0" "idle\0" "defunct\0" "stop\0" "uwait\0"
/* error messages */
	"/proc unavailable\0"
	"intilization failed\0"
/* fields */
	"uid\0"
	"gid\0"
	"pid\0"
	"fname\0"
	"ppid\0"
	"pgrp\0"
	"sess\0"
	"ttynum\0"
	"flags\0"
	"minflt\0"
	"cminflt\0"
	"majflt\0"
	"cmajflt\0"
	"utime\0"
	"stime\0"
	"cutime\0"
	"cstime\0"
	"priority\0"
	"start\0"
	"size\0"
	"rss\0"
	"wchan\0"
	"time\0"
	"ctime\0"
	"state\0"
	"euid\0"
	"suid\0"
	"fuid\0"
	"egid\0"
	"sgid\0"
	"fgid\0"
	"pctcpu\0"
	"pctmem\0"
	"cmndline\0"
	"exec\0"
	"cwd\0"
/* format string */
	"IIISIIIILLLLLJJJJIJLLLJJSIIIIIISSSSS\0"
};

/* I generated this array with a perl script processing the above char array,
 * and then performed cross string optimization (by hand) for: 
 *		pid,	time,	uid,	gid,	minflt,		majflt,
 */
static const size_t strings_index[] =
{
/* process status strings */
	0, 6, 11, 15, 20, 28, 33,
/* error messages */
	39,	57,
/* fields */
	77,
	81,
	85,
	89,
	95,
	100,
	105,
	110,
	117,
	123,
	130,
	138,
	145,
	153,
	159,
	165,
	172,
	179,
	188,
	194,
	199,
	203,
	209,
	214,
	220,
	226,
	231,
	236,
	241,
	246,
	251,
	256,
	263,
	270,
	279,
	284,
/* default format string (pre lower casing) */
	288
};

enum string_name {
	/* NOTE: we start this enum at 7, so we can use still keep using the
  	 * state enum, and have those correspond to the static strings */
/* error messages */
	STR_ERR_PROC_STATFS = 7,
	STR_ERR_INIT,
/* fields */
	STR_FIELD_UID,
	STR_FIELD_GID,
	STR_FIELD_PID,
	STR_FIELD_FNAME,
	STR_FIELD_PPID,
	STR_FIELD_PGRP,
	STR_FIELD_SESS,
	STR_FIELD_TTYNUM,
	STR_FIELD_FLAGS,
	STR_FIELD_MINFLT,
	STR_FIELD_CMINFLT,
	STR_FIELD_MAJFLT,
	STR_FIELD_CMAJFLT,
	STR_FIELD_UTIME,
	STR_FIELD_STIME,
	STR_FIELD_CUTIME,
	STR_FIELD_CSTIME,
	STR_FIELD_PRIORITY,
	STR_FIELD_START,
	STR_FIELD_SIZE,
	STR_FIELD_RSS,
	STR_FIELD_WCHAN,
	STR_FIELD_TIME,
	STR_FIELD_CTIME,
	STR_FIELD_STATE,
	STR_FIELD_EUID,
	STR_FIELD_SUID,
	STR_FIELD_FUID,
	STR_FIELD_EGID,
	STR_FIELD_SGID,
	STR_FIELD_FGID,
	STR_FIELD_PCTCPU,
	STR_FIELD_PCTMEM,
	STR_FIELD_CMNDLINE,
	STR_FIELD_EXEC,
	STR_FIELD_CWD,
/* format string */
	STR_DEFAULT_FORMAT
};


enum field
{
	F_UID,
	F_GID,
	F_PID,
	F_FNAME,
	F_PPID,
	F_PGRP,
	F_SESS,
	F_TTYNUM,
	F_FLAGS,
	F_MINFLT,
	F_CMINFLT,
	F_MAJFLT,
	F_CMAJFLT,
	F_UTIME,
	F_STIME,
	F_CUTIME,
	F_CSTIME,
	F_PRIORITY,
	F_START,
	F_SIZE,
	F_RSS,
	F_WCHAN,
	F_TIME,
	F_CTIME,
	F_STATE,
	F_EUID,
	F_SUID,
	F_FUID,
	F_EGID,
	F_SGID,
	F_FGID,
	F_PCTCPU,
	F_PCTMEM,
	F_CMNDLINE,
	F_EXEC,
	F_CWD
};

static const char* const field_names[] =
{
	strings + 77,
	strings + 81,
	strings + 85,
	strings + 89,
	strings + 95,
	strings + 100,
	strings + 105,
	strings + 110,
	strings + 117,
	strings + 123,
	strings + 130,
	strings + 138,
	strings + 145,
	strings + 153,
	strings + 159,
	strings + 165,
	strings + 172,
	strings + 179,
	strings + 188,
	strings + 194,
	strings + 199,
	strings + 203,
	strings + 209,
	strings + 214,
	strings + 220,
	strings + 226,
	strings + 231,
	strings + 236,
	strings + 241,
	strings + 246,
	strings + 251,
	strings + 256,
	strings + 263,
	strings + 270,
	strings + 279,
	strings + 284
};

