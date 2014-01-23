
void io_hlt(void);

void HariMain(void)
{

fin:
	io_hlt();  /* execute the _io_hlt assemble function*/
	goto fin;
}
