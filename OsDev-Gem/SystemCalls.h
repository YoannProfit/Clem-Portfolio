class Syscalls 
{
	public:
		void	init();
		void	add(u32 num,syscall_handler h);
		void	call(u32 num);
		
	protected:
		syscall_handler		calls[NB_SYSCALLS];

};