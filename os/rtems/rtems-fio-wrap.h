#ifdef __rtems__

void act_register(void);
void tiobench_register(void);
void fio_syncio_register(void);
void fio_filecreate_register(void);
void fio_null_register(void);
void prio_tree_init(void);
void fio_syncio_register_ft(void);
void fio_client_hash_init(void);
void fio_syncio_unregister(void);
void tiobench_unregister(void);
void fio_filecreate_unregister(void);
void fio_null_unregister(void);
void act_unregister(void);
void fio_syncio_unregister_ft(void);

static int
mainwrapper(int argc, char *argv[])
{
    int err=0;

    /* Constructors */
	act_register();
	tiobench_register();
	fio_syncio_register();
	fio_filecreate_register();
	fio_null_register();
	prio_tree_init();
	fio_syncio_register_ft();
	fio_client_hash_init();

	err = main(argc, argv, (char *)NULL);

	/* Destructors */
	fio_syncio_unregister();
    tiobench_unregister();
    fio_filecreate_unregister();
    fio_null_unregister();
    act_unregister();
    fio_syncio_unregister_ft();

    return err;
}

rtems_bsd_command_fio(int argc, char *argv[])
{
    int exit_code;

    exit_code = mainwrapper(argc, argv);

    return exit_code;
}
#endif /* __rtems__ */
