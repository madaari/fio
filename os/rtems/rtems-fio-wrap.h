#ifndef RTEMS_FIO_WRAP
#define RTEMS_FIO_WRAP
/* RTEMS specific wrapper for explicitly calling constructors and
 * destructors */

void fio_syncio_register(void);
void fio_filecreate_register(void);
void fio_null_register(void);
void fio_syncio_unregister(void);
void fio_filecreate_unregister(void);
void fio_null_unregister(void);

static int
mainwrapper(int argc, char *argv[])
{
	int err=0;

	/* Constructors */
	fio_syncio_register();
	fio_filecreate_register();
	fio_null_register();

	err = main(argc, argv, NULL);

	/* Destructors */
	fio_syncio_unregister();
	fio_filecreate_unregister();
	fio_null_unregister();

	return err;
}

rtems_bsd_command_fio(int argc, char *argv[])
{
	int exit_code;

	exit_code = mainwrapper(argc, argv);

	return exit_code;
}
#endif /* RTEMS_FIO_WRAP */
