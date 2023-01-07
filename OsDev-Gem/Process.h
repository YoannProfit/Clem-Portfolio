namespace Kernel
{
		u32		create(char* file, int argc, char **argv);
		void	sendSignal(int sig);
		u32		wait();
		
		u32 	addFile(File* fp,u32 m);
		File*	getFile(u32 fd);
		void	deleteFile(u32 fd);
		openfile*	getFileInfo(u32 fd);
		
		void	exit();
		int		fork();
		
		
		void	setState(u8 st);
		u8		getState();
		void	setFile(u32 fd,File* fp,u32 ptr, u32 mode);
		void	setPid(u32 st);
		u32		getPid();
		
		void			setPNext(Process* p);
		
		Process* 		schedule();
		Process*		getPNext();
		Process*		getPParent();
		process_st* 	getPInfo();
		void			setPParent(Process*p);
		
		void			reset_pinfo();
		
		process_st		info;
		
		File*	getCurrentDir();
		void	setCurrentDir(File* f);
		
	protected:
		static u32	proc_pid;
		
		u32 		pid;
		u8			state;
		Process*	pparent;
		Process*	pnext;
		openfile	openfp[CONFIG_MAX_FILE];
		proc_info	ppinfo;
		File*		cdir;
		
		Buffer*		ipc;
		
		static char*	default_tty;
}



