#include "server.h"

static void
calc_server_class_init (CalcServerClass * klass)
{
  GError *error = NULL;

  g_assert (klass != NULL);
  /* Init the DBus connection, per-klass */
  klass->connection = dbus_g_bus_get (DBUS_BUS_SESSION, &error);
  if (klass->connection == NULL)
    {
      g_warning ("Unable to connect to dbus: %s", error->message);
      g_error_free (error);
      return;
    }

  /* &dbus_glib__object_info is provided in the server-bindings.h file */
  /* OBJECT_TYPE_SERVER is the GType of your server object */
  dbus_g_object_type_install_info (TYPE_CALC_SERVER,
				   &dbus_glib_calc_object_info);

  g_message ("CalcServerClass successfully initialized");
}

static gint
calc_server_init (CalcServer * server)
{
  GError *error = NULL;
  DBusGProxy *driver_proxy;
  CalcServerClass *klass = CALC_SERVER_GET_CLASS (server);
  guint request_ret;
  gint retcode = 1;

  /* Register DBUS path */
  dbus_g_connection_register_g_object (klass->connection,
				       CALC_SERVICE_OBJECT_PATH,
				       G_OBJECT (server));

  driver_proxy = dbus_g_proxy_new_for_name (klass->connection,
					    DBUS_SERVICE_DBUS,
					    DBUS_PATH_DBUS,
					    DBUS_INTERFACE_DBUS);

  if (!org_freedesktop_DBus_request_name (driver_proxy, CALC_SERVICE_NAME, 0, &request_ret,&error))
    {
      g_warning ("Unable to register service: %s", error->message);
      g_error_free (error);
      retcode = 0;
    }
  g_object_unref (driver_proxy);

  return (retcode);
}

GType
calc_server_get_type (void)
{
  static GType server_type = 0;
  if (!server_type)
    {
      static const GTypeInfo server_info = {
	sizeof (CalcServerClass),	/* class structure size */
	NULL,			/* base class initializer */
	NULL,			/* base class finalizer */
	(GClassInitFunc) calc_server_class_init,	/* class initializer */
	NULL,			/* class finalizer */
	NULL,			/* class data */
	sizeof (CalcServer),	/* instance structure size */
	1,			/* preallocated instances */
	NULL,			/* instance initializers */
	NULL
      };

      server_type = g_type_register_static (G_TYPE_OBJECT,	/* parent class */
					    "CalcServer", &server_info, 0);
    }

  return server_type;
}

gboolean
calc_add (CalcServer * obj, gint first, gint second, gint * sum,
	  GError ** error)
{
  g_assert (obj != NULL);
  g_assert (sum != NULL);
  *sum = first + second;
  return TRUE;
}

gboolean
calc_sub (CalcServer * obj, gint minued, gint sub, gint * diff,
	  GError ** error)
{
  g_assert (obj != NULL);
  g_assert (diff != NULL);
  *diff = minued - sub;
  return TRUE;
}

int
main ()
{
  CalcServer *server;
  GMainLoop *loop;

  dbus_g_thread_init ();
  loop = g_main_loop_new (NULL, FALSE);
  server = g_object_new (TYPE_CALC_SERVER, NULL);

  if (calc_server_init (server))
    {
      g_main_loop_run (loop);
    }
  return 0;
}
