#include <config.h>

#include <stdio.h>
#include <string.h> 

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#if defined(__linux__)
#include <elf.h>
#include <link.h>
#endif


#if HAVE_SYS_UTSNAME_H
#include <sys/utsname.h>
#endif

#if HAVE_SYS_SYSTEMCFG_H
#include <sys/systemcfg.h>
#else
#define __power_pc() 0
#endif

#ifdef HAVE_SYS_AUXV_H
#include <sys/auxv.h>
#endif

//#define __i386__ 1

#	if defined(__linux__) && defined(__i386__)
#include <setjmp.h>
#include <signal.h>

/*
 * Generic CPUID function
 */
static inline void cpuid(unsigned int op, unsigned int *eax, unsigned int *ebx, unsigned int *ecx, unsigned int *edx)
{
    asm volatile (
	"pushl	%%ebx		\n"
	"cpuid			\n"
	"movl	%%ebx,	%%esi	\n"
	"popl	%%ebx		\n"
    : "=a" (*eax), "=S" (*ebx), "=c" (*ecx), "=d" (*edx)
    : "a" (op));
}

/*
 * CPUID functions returning a single datum
 */
static inline unsigned int cpuid_eax(unsigned int op)
{
	unsigned int tmp, val;
	cpuid(op, &val, &tmp, &tmp, &tmp);
	return val;
}

static inline unsigned int cpuid_ebx(unsigned int op)
{
	unsigned int tmp, val;
	cpuid(op, &tmp, &val, &tmp, &tmp);
	return val;
}

static inline unsigned int cpuid_ecx(unsigned int op)
{
	unsigned int tmp, val;
	cpuid(op, &tmp, &tmp, &val, &tmp);
	return val;
}

static inline unsigned int cpuid_edx(unsigned int op)
{
	unsigned int tmp, val;
	cpuid(op, &tmp, &tmp, &tmp, &val);
	return val;
}

static sigjmp_buf jenv;

static inline void model3(__attribute__((unused)) int _unused)
{
	siglongjmp(jenv, 1);
}

static inline int RPMClass(void)
{
	int cpu;
	unsigned int tfms, junk, cap, capamd;
	struct sigaction oldsa;
	
	sigaction(SIGILL, NULL, &oldsa);
	signal(SIGILL, model3);
	
	if (sigsetjmp(jenv, 1)) {
		sigaction(SIGILL, &oldsa, NULL);
		return 3;
	}
		
	if (cpuid_eax(0x000000000)==0) {
		sigaction(SIGILL, &oldsa, NULL);
		return 4;
	}

	cpuid(0x00000001, &tfms, &junk, &junk, &cap);
	cpuid(0x80000001, &junk, &junk, &junk, &capamd);
	
	cpu = (tfms>>8)&15;
	
	if (cpu == 5
	    && cpuid_ecx(0) == *(uint32_t*)"68xM"
	    && cpuid_edx(0) == *(uint32_t*)"Teni"
	    && (cpuid_edx(1) & ((1<<8)|(1<<15))) == ((1<<8)|(1<<15))) {
		sigaction(SIGILL, &oldsa, NULL);
		return 6;	/* has CX8 and CMOV */
	}

	sigaction(SIGILL, &oldsa, NULL);

#define USER686 ((1<<4) | (1<<8) | (1<<15))
	/* Transmeta Crusoe CPUs say that their CPU family is "5" but they have enough features for i686. */
	if (cpu == 5 && (cap & USER686) == USER686)
		return 6;

	if (cpu < 6)
		return cpu;
		
	if (cap & (1<<15)) {
		/* CMOV supported? */
		if (capamd & (1<<30))
			return 7;	/* 3DNOWEXT supported */
		return 6;
	}
		
	return 5;
}

/* should only be called for model 6 CPU's */
static int is_athlon(void)
{
	unsigned int eax, ebx, ecx, edx;
	char vendor[16];
	int i;
	
	cpuid (0, &eax, &ebx, &ecx, &edx);

 	memset(vendor, 0, sizeof(vendor));
 	
 	for (i=0; i<4; i++)
 		vendor[i] = (char) (ebx >>(8*i));
 	for (i=0; i<4; i++)
 		vendor[4+i] = (char) (edx >>(8*i));
 	for (i=0; i<4; i++)
 		vendor[8+i] = (char) (ecx >>(8*i));
 		
 	if (strncmp(vendor, "AuthenticAMD", 12))  
 		return 0;

	return 1;
}

static int is_pentium3(void)
{
    unsigned int eax, ebx, ecx, edx, family, model;
    char vendor[16];
    cpuid(0, &eax, &ebx, &ecx, &edx);
    memset(vendor, 0, sizeof(vendor));
    *((unsigned int *)&vendor[0]) = ebx;
    *((unsigned int *)&vendor[4]) = edx;
    *((unsigned int *)&vendor[8]) = ecx;
    if (strncmp(vendor, "GenuineIntel", 12))
	return 0;
    cpuid(1, &eax, &ebx, &ecx, &edx);
    family = (eax >> 8) & 0x0f;
    model = (eax >> 4) & 0x0f;
    if (family == 6)
	switch (model)
	{
	    case 7:	// Pentium III, Pentium III Xeon (model 7)
	    case 8:	// Pentium III, Pentium III Xeon, Celeron (model 8)
	    case 9:	// Pentium M
	    case 10:	// Pentium III Xeon (model A)
	    case 11:	// Pentium III (model B)
		return 1;
	}
    return 0;
}

static int is_pentium4(void)
{
    unsigned int eax, ebx, ecx, edx, family, model;
    char vendor[16];
    cpuid(0, &eax, &ebx, &ecx, &edx);
    memset(vendor, 0, sizeof(vendor));
    *((unsigned int *)&vendor[0]) = ebx;
    *((unsigned int *)&vendor[4]) = edx;
    *((unsigned int *)&vendor[8]) = ecx;
    if (strncmp(vendor, "GenuineIntel", 12))
	return 0;
    cpuid(1, &eax, &ebx, &ecx, &edx);
    family = (eax >> 8) & 0x0f;
    model = (eax >> 4) & 0x0f;
    if (family == 15)
	switch (model)
	{
	    case 0:	// Pentium 4, Pentium 4 Xeon                 (0.18um)
	    case 1:	// Pentium 4, Pentium 4 Xeon MP, Celeron     (0.18um)
	    case 2:	// Pentium 4, Mobile Pentium 4-M,
			// Pentium 4 Xeon, Pentium 4 Xeon MP,
			// Celeron, Mobile Celron                    (0.13um)
	    case 3:	// Pentium 4, Celeron                        (0.09um)
		return 1;
	}
    return 0;
}

static int is_geode(void)
{
    unsigned int eax, ebx, ecx, edx, family, model;
    char vendor[16];
    memset(vendor, 0, sizeof(vendor));
    
    cpuid(0, &eax, &ebx, &ecx, &edx);
    memset(vendor, 0, sizeof(vendor));
    *((unsigned int *)&vendor[0]) = ebx;
    *((unsigned int *)&vendor[4]) = edx;
    *((unsigned int *)&vendor[8]) = ecx;
    if (strncmp(vendor, "AuthenticAMD", 12))
        return 0;
    cpuid(1, &eax, &ebx, &ecx, &edx);
    family = (eax >> 8) & 0x0f;
    model = (eax >> 4) & 0x0f;
    if (family == 5)
	switch (model)
	{
            case 10: // Geode 
                return 1;
        }
    return 0;
}
#endif

static struct rpmat_s {
    const char *platform;
    uint64_t hwcap;
    uint64_t hwcap2;
} rpmat;

#if defined(__linux__)
#ifndef AT_HWCAP2 /* glibc < 2.18 */
#define AT_HWCAP2 26
#endif
/**
 * Populate rpmat structure with auxv values
 */
static void read_auxv(void)
{
    static int oneshot = 1;

    if (oneshot) {
#ifdef HAVE_GETAUXVAL
	rpmat.platform = (char *) getauxval(AT_PLATFORM);
	if (!rpmat.platform)
	    rpmat.platform = "";
	rpmat.hwcap = getauxval(AT_HWCAP);
	rpmat.hwcap2 = getauxval(AT_HWCAP2);
#else
	rpmat.platform = "";
	int fd = open("/proc/self/auxv", O_RDONLY);

	if (fd == -1) {
	    //rpmlog(RPMLOG_WARNING,
	    //   _("Failed to read auxiliary vector, /proc not mounted?\n"));
            return;
	} else {
	    ElfW(auxv_t) auxv;
	    while (read(fd, &auxv, sizeof(auxv)) == sizeof(auxv)) {
                switch (auxv.a_type)
                {
                    case AT_NULL:
                        break;
                    case AT_PLATFORM:
                        rpmat.platform = strdup((char *) auxv.a_un.a_val);
                        break;
                    case AT_HWCAP:
                        rpmat.hwcap = auxv.a_un.a_val;
                        break;
		    case AT_HWCAP2:
			rpmat.hwcap2 = auxv.a_un.a_val;
			break;
                }
	    }
	    close(fd);
	}
#endif
	oneshot = 0; /* only try once even if it fails */
    }
    return;
}
#endif

/**
 */

static struct utsname un;

static void autodetect()
{
    char * chptr;
    //canonEntry canon;
    int rc;

#if defined(__linux__)
    /* Populate rpmat struct with hw info */
    read_auxv();
#endif

    {
	rc = uname(&un);
	if (rc < 0) {
	    strcpy(un.machine, ""); // XXX
	}

#if !defined(__linux__)
	if (!strcmp(un.sysname, "AIX")) {
	    strcpy(un.machine, __power_pc() ? "ppc" : "rs6000");
	    sprintf(un.sysname,"aix%s.%s", un.version, un.release);
	}
	else if (!strcmp(un.sysname, "Darwin")) { 
#if defined(__ppc__)
	    strcpy(un.machine, "ppc");
#elif defined(__i386__)
	    strcpy(un.machine, "i386");
#elif defined(__x86_64__)
	    strcpy(un.machine, "x86_64");
#else
	    #warning "No architecture defined! Automatic detection may not work!"
#endif 
	}
	else if (!strcmp(un.sysname, "SunOS")) {
	    /* Solaris 2.x: n.x.x becomes n-3.x.x */
	    sprintf(un.sysname, "solaris%1d%s", atoi(un.release)-3,
		    un.release+1+(atoi(un.release)/10));

	    /* Solaris on Intel hardware reports i86pc instead of i386
	     * (at least on 2.6 and 2.8)
	     */
	    if (!strcmp(un.machine, "i86pc"))
		sprintf(un.machine, "i386");
	}
	else if (!strcmp(un.sysname, "HP-UX"))
	    /*make un.sysname look like hpux9.05 for example*/
	    sprintf(un.sysname, "hpux%s", strpbrk(un.release, "123456789"));
	else if (!strcmp(un.sysname, "OSF1"))
	    /*make un.sysname look like osf3.2 for example*/
	    sprintf(un.sysname, "osf%s", strpbrk(un.release, "123456789"));
#endif	/* __linux__ */

	/* get rid of the hyphens in the sysname */
	for (chptr = un.machine; *chptr != '\0'; chptr++)
	    if (*chptr == '/') *chptr = '-';

#	if defined(__MIPSEL__) || defined(__MIPSEL) || defined(_MIPSEL)
	    /* little endian */
#		if defined(__mips64)
		    /* 64-bit */
#			if !defined(__mips_isa_rev) || __mips_isa_rev < 6
			    /* r1-r5 */
			    strcpy(un.machine, "mips64el");
#			else
			    /* r6 */
			    strcpy(un.machine, "mips64r6el");
#			endif
#		else
		    /* 32-bit */
#			if !defined(__mips_isa_rev) || __mips_isa_rev < 6
			    /* r1-r5 */
			    strcpy(un.machine, "mipsel");
#			else
			    /* r6 */
			    strcpy(un.machine, "mipsr6el");
#			endif
#		endif
#	elif defined(__MIPSEB__) || defined(__MIPSEB) || defined(_MIPSEB)
	   /* big endian */
#		if defined(__mips64)
		    /* 64-bit */
#			if !defined(__mips_isa_rev) || __mips_isa_rev < 6
			    /* r1-r5 */
			    strcpy(un.machine, "mips64");
#			else
			    /* r6 */
			    strcpy(un.machine, "mips64r6");
#			endif
#		else
		    /* 32-bit */
#			if !defined(__mips_isa_rev) || __mips_isa_rev < 6
			    /* r1-r5 */
			    strcpy(un.machine, "mips");
#			else
			    /* r6 */
			    strcpy(un.machine, "mipsr6");
#			endif
#		endif
#	endif

#if defined(__linux__)
	/* in linux, lets rename parisc to hppa */
	if (!strcmp(un.machine, "parisc"))
	    strcpy(un.machine, "hppa");
#endif

#	if defined(__hpux) && defined(_SC_CPU_VERSION)
	{
#	    if !defined(CPU_PA_RISC1_2)
#                define CPU_PA_RISC1_2  0x211 /* HP PA-RISC1.2 */
#           endif
#           if !defined(CPU_PA_RISC2_0)
#               define CPU_PA_RISC2_0  0x214 /* HP PA-RISC2.0 */
#           endif
	    int cpu_version = sysconf(_SC_CPU_VERSION);

#	    if defined(CPU_HP_MC68020)
		if (cpu_version == CPU_HP_MC68020)
		    strcpy(un.machine, "m68k");
#	    endif
#	    if defined(CPU_HP_MC68030)
		if (cpu_version == CPU_HP_MC68030)
		    strcpy(un.machine, "m68k");
#	    endif
#	    if defined(CPU_HP_MC68040)
		if (cpu_version == CPU_HP_MC68040)
		    strcpy(un.machine, "m68k");
#	    endif

#	    if defined(CPU_PA_RISC1_0)
		if (cpu_version == CPU_PA_RISC1_0)
		    strcpy(un.machine, "hppa1.0");
#	    endif
#	    if defined(CPU_PA_RISC1_1)
		if (cpu_version == CPU_PA_RISC1_1)
		    strcpy(un.machine, "hppa1.1");
#	    endif
#	    if defined(CPU_PA_RISC1_2)
		if (cpu_version == CPU_PA_RISC1_2)
		    strcpy(un.machine, "hppa1.2");
#	    endif
#	    if defined(CPU_PA_RISC2_0)
		if (cpu_version == CPU_PA_RISC2_0)
		    strcpy(un.machine, "hppa2.0");
#	    endif
	}
#	endif	/* hpux */

#	if defined(__linux__) && defined(__sparc__)
#	if !defined(HWCAP_SPARC_BLKINIT)
#	    define HWCAP_SPARC_BLKINIT	0x00000040
#	endif
	if (!strcmp(un.machine, "sparc")) {
	    #define PERS_LINUX		0x00000000
	    #define PERS_LINUX_32BIT	0x00800000
	    #define PERS_LINUX32	0x00000008

	    extern int personality(unsigned long);
	    int oldpers;
	    
	    oldpers = personality(PERS_LINUX_32BIT);
	    if (oldpers != -1) {
		if (personality(PERS_LINUX) != -1) {
		    uname(&un);
		    if (!strcmp(un.machine, "sparc64")) {
			strcpy(un.machine, "sparcv9");
			oldpers = PERS_LINUX32;
		    }
		}
		personality(oldpers);
	    }

	    /* This is how glibc detects Niagara so... */
	    if (rpmat.hwcap & HWCAP_SPARC_BLKINIT) {
		if (!strcmp(un.machine, "sparcv9") || !strcmp(un.machine, "sparc")) {
		    strcpy(un.machine, "sparcv9v");
		} else if (!strcmp(un.machine, "sparc64")) {
		    strcpy(un.machine, "sparc64v");
		}
	    }
	}
#	endif	/* sparc*-linux */

#	if defined(__linux__) && defined(__powerpc__)
#	if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
	{
            int powerlvl;
            if (!!strcmp(un.machine, "ppc") &&
		    sscanf(rpmat.platform, "power%d", &powerlvl) == 1 &&
		    powerlvl > 6) {
                strcpy(un.machine, "ppc64p7");
	    }
        }
#	endif	/* __ORDER_BIG_ENDIAN__ */
#	endif	/* ppc64*-linux */

#	if defined(__linux__) && defined(__arm__) && defined(__ARM_PCS_VFP)
#	if !defined(HWCAP_ARM_VFP)
#	    define HWCAP_ARM_VFP	(1 << 6)
#	endif
#	if !defined(HWCAP_ARM_NEON)
#	    define HWCAP_ARM_NEON	(1 << 12)
#	endif
#	if !defined(HWCAP_ARM_VFPv3)
#	    define HWCAP_ARM_VFPv3	(1 << 13)
#	endif
#	if !defined(HWCAP2_AES)
#	    define HWCAP2_AES		(1 << 0)
#	endif
	/*
	 * un.machine is armvXE, where X is version number and E is
	 * endianness (b or l)
	 */
	if (!strcmpn(un.machine, "armv", 4)) {
		char endian = un.machine[strlen(un.machine)-1];
		char *modifier = un.machine + 5;
		/* keep armv7, armv8, armv9, armv10, ... */
		while(risdigit(*modifier)) 
			modifier++;
		if (rpmat.hwcap & HWCAP_ARM_VFPv3)
			*modifier++ = 'h';
		if (rpmat.hwcap2 & HWCAP2_AES)
			*modifier++ = 'c';
		if (rpmat.hwcap & HWCAP_ARM_NEON)
			*modifier++ = 'n';
		*modifier++ = endian;
		*modifier++ = 0;
	}
#	endif	/* arm*-linux */

#	if defined(__linux__) && defined(__riscv__)
	if (!strcmp(un.machine, "riscv")) {
		if (sizeof(long) == 4)
			strcpy(un.machine, "riscv32");
		else if (sizeof(long) == 8)
			strcpy(un.machine, "riscv64");
		else if (sizeof(long) == 16)
			strcpy(un.machine, "riscv128");
	}
#	endif	/* riscv */

#	if defined(__GNUC__) && defined(__alpha__)
	{
	    unsigned long amask, implver;
	    register long v0 __asm__("$0") = -1;
	    __asm__ (".long 0x47e00c20" : "=r"(v0) : "0"(v0));
	    amask = ~v0;
	    __asm__ (".long 0x47e03d80" : "=r"(v0));
	    implver = v0;
	    switch (implver) {
	    case 1:
	    	switch (amask) {
	    	case 0: strcpy(un.machine, "alphaev5"); break;
	    	case 1: strcpy(un.machine, "alphaev56"); break;
	    	case 0x101: strcpy(un.machine, "alphapca56"); break;
	    	}
	    	break;
	    case 2:
	    	switch (amask) {
	    	case 0x303: strcpy(un.machine, "alphaev6"); break;
	    	case 0x307: strcpy(un.machine, "alphaev67"); break;
	    	}
	    	break;
	    }
	}
#	endif

#	if defined(__linux__) && defined(__i386__)
	{
	    char mclass = (char) (RPMClass() | '0');

	    if ((mclass == '6' && is_athlon()) || mclass == '7')
	    	strcpy(un.machine, "athlon");
	    else if (is_pentium4())
		strcpy(un.machine, "pentium4");
	    else if (is_pentium3())
		strcpy(un.machine, "pentium3");
	    else if (is_geode())
		strcpy(un.machine, "geode");
	    else if (strchr("3456", un.machine[1]) && un.machine[1] != mclass)
		un.machine[1] = mclass;
	}
#	endif

	/* the uname() result goes through the arch_canon table */
	/*
	canon = lookupInCanonTable(un.machine,
			   ctx->tables[RPM_MACHTABLE_INSTARCH].canons,
			   ctx->tables[RPM_MACHTABLE_INSTARCH].canonsLength);
	if (canon)
	    rstrlcpy(un.machine, canon->short_name, sizeof(un.machine));

	canon = lookupInCanonTable(un.sysname,
			   ctx->tables[RPM_MACHTABLE_INSTOS].canons,
			   ctx->tables[RPM_MACHTABLE_INSTOS].canonsLength);
	if (canon)
	    rstrlcpy(un.sysname, canon->short_name, sizeof(un.sysname));
	ctx->machDefaults = 1;
	break;
	*/
    }

}

const char * libarch_autodetect_arch()
{
    if (un.machine[0] == '\0')
	autodetect();
    return un.machine;
}
const char * libarch_autodetect_os()
{
    if (un.machine[0] == '\0')
	autodetect();
    return un.sysname;
}
