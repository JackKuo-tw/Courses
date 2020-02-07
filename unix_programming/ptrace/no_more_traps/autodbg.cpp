#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <fstream>

#include "ptools.h"

using namespace std;
fstream f;

void errquit(const char *msg) {
	perror(msg);
	exit(-1);
}

unsigned int read_one_opcode() {
	char tmp[2];
	f.read((char*)(&tmp), (int)sizeof(tmp));
	return (unsigned int)strtoul(tmp, NULL , 16);
}

void dump_code(long addr, long code) {
	fprintf(stderr, "## %lx: code = %02x %02x %02x %02x %02x %02x %02x %02x\n",
		addr,
		((unsigned char *) (&code))[0],
		((unsigned char *) (&code))[1],
		((unsigned char *) (&code))[2],
		((unsigned char *) (&code))[3],
		((unsigned char *) (&code))[4],
		((unsigned char *) (&code))[5],
		((unsigned char *) (&code))[6],
		((unsigned char *) (&code))[7]);
}

int main(int argc, char *argv[]) {
	f.open("no_more_traps.txt", ios::in);
	pid_t child;
	long offset = 0x9cc; // the default value we reverse engineered
	if(argc > 1) {
		offset = strtol(argv[1], NULL, 0);
	}
	fprintf(stderr, "## offset = %ld (0x%lx)\n", offset, offset);
	if((child = fork()) < 0) errquit("fork");
	if(child == 0) {
		if(ptrace(PTRACE_TRACEME, 0, 0, 0) < 0) errquit("ptrace");
		execlp("./no_more_traps", "./no_more_traps", NULL);
		errquit("execvp");
	} else {
		int status;
		unsigned long baseaddr, target, code;
		map<range_t, map_entry_t> vmmap;
		map<range_t, map_entry_t>::iterator vi;

		if(waitpid(child, &status, 0) < 0) errquit("waitpid");
		assert(WIFSTOPPED(status));
		ptrace(PTRACE_SETOPTIONS, child, 0, PTRACE_O_EXITKILL);

		if(load_maps(child, vmmap) <= 0) {
			fprintf(stderr, "## cannot load memory mappings.\n");
			return -1;
		}
		fprintf(stderr, "## %zu map entries loaded.\n", vmmap.size());

		for(vi = vmmap.begin(); vi != vmmap.end(); vi++) {
			if(vi->second.name == "no_more_traps"
			&& vi->second.offset == 0
			&& (vi->second.perm & 0x01) == 0x01) {
				baseaddr = vi->second.range.begin;
				break;
			}
		}
		target = baseaddr;
		fprintf(stderr, "## baseaddr = 0x%zx, target = 0x%zx.\n", baseaddr, target);

		/* get original text: 48 39 d0 */
//		code = ptrace(PTRACE_PEEKTEXT, child, target, 0);
//		dump_code(target, code);

		/* continue the execution */
		ptrace(PTRACE_CONT, child, 0, 0);

		while(waitpid(child, &status, 0) > 0) {
			struct user_regs_struct regs;
			if(!WIFSTOPPED(status)) continue;
			if(ptrace(PTRACE_GETREGS, child, 0, &regs) != 0)
				errquit("ptrace(GETREGS)");
//			printf("rip: %llx %lx\n", regs.rip, regs.rip-1);
//			sleep(1);
			unsigned long long int current_addr = regs.rip - 1;
			code = ptrace(PTRACE_PEEKTEXT, child, current_addr, 0);
			// if the begin of instruction is 0xcc
			if((code & 0xff) == 0xcc) {
//				dump_code((long)current_addr, code);
				((unsigned char *) (&code))[0] = read_one_opcode();
//				dump_code((long)current_addr, code);
				if(ptrace(PTRACE_POKETEXT, child, current_addr, code) != 0)
					errquit("ptrace(POKETEXT)");
				/* set registers */
				regs.rip = current_addr;
				if(ptrace(PTRACE_SETREGS, child, 0, &regs) != 0) errquit("ptrace(SETREGS)");
			}
			ptrace(PTRACE_CONT, child, 0, 0);
		}
		printf("done\n");
	}
	return 0;
}


