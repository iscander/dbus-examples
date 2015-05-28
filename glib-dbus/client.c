#include "client.h"

static void
calc_client_class_init (CalcClientClass * klass)
{
  GError *error = NULL;

  /* Init the DBus connection, per-klass */
  klass->connection = dbus_g_bus_get (DBUS_BUS_SESSION, &error);
  if (klass->connection == NULL)
    {
      g_warning ("Unable to connect to dbus: %s", error->message);
      g_error_free (error);
      return;
    }

/* This won't trigger activation! */
  klass->proxy = dbus_g_proxy_new_for_name (klass->connection,
					    CALC_SERVICE_NAME,
					    CALC_SERVICE_OBJECT_PATH,
					    CALC_SERVICE_INTERFACE);

  g_message ("CalcClientClass successfully initialized");
}


GType
calc_client_get_type (void)
{
  static GType client_type = 0;

  if (!client_type)
    {
      static const GTypeInfo client_info = {
	sizeof (CalcClientClass),	/* class structure size */
	NULL,			/* base class initializer */
	NULL,			/* base class finalizer */
	(GClassInitFunc) calc_client_class_init,	/* class initializer */
	NULL,			/* class finalizer */
	NULL,			/* class data */
	sizeof (CalcClient),	/* instance structure size */
	1,			/* preallocated instances */
	NULL,			/* instance initializers */
	NULL
      };

      client_type = g_type_register_static (G_TYPE_OBJECT,	/* parent class */
					    "CalcClient", &client_info, 0);
    }

  return client_type;
}

static void
client_add_reply (DBusGProxy * proxy, gint sum, GError * error,
		  gpointer userdata)
{
  if (error != NULL)
    {
      g_warning ("An error occured while calling add remote method: %s",
		 error->message);
      g_error_free (error);
      return;
    }

  g_print ("Sum is: %d\n", sum);
}

static void
client_sub_reply (DBusGProxy * proxy, gint diff, GError * error,
		  gpointer userdata)
{
  if (error != NULL)
    {
      g_warning ("An error occured while calling sub remote method: %s",
		 error->message);
      g_error_free (error);
      return;
    }

  g_print ("Difference is: %d\n", diff);
}

static void
client_add_async (CalcClient * client, const gint first, const gint second)
{
  CalcClientClass *klass = CALC_CLIENT_GET_CLASS (client);

/* The method call will trigger activation and prints results asynchronously */
  org_test_Calck_add_async (klass->proxy,
			    first, second, client_add_reply, client);
}

static void
client_add_sync (CalcClient * client, const gint first, const gint second)
{
  GError *error = NULL;
  CalcClientClass *klass = CALC_CLIENT_GET_CLASS (client);
  gint sum;
/* The method call will trigger activation and blocks until result arives */
  if (!org_test_Calck_add (klass->proxy, first, second, &sum, &error))
    {
      /* Method failed, the GError is set, let's warn everyone */
      g_warning ("Woops remote sync add failed: %s", error->message);
      g_error_free (error);
      return;
    }
  g_print ("Sync request: %d + %d = %d\n", first, second, sum);
}

static void
client_sub_async (CalcClient * client, const gint minuend,
		  const gint subtrahend)
{
  CalcClientClass *klass = CALC_CLIENT_GET_CLASS (client);
/* The method call will trigger activation and prints results asynchronously */
  org_test_Calck_sub_async (klass->proxy,
			    minuend, subtrahend, client_sub_reply, client);
}

static void
client_sub_sync (CalcClient * client, const gint minuend,
		 const gint subtrahend)
{
  GError *error = NULL;
  CalcClientClass *klass = CALC_CLIENT_GET_CLASS (client);
  gint diff;
/* The method call will trigger activation and blocks until result arives */
  if (!org_test_Calck_sub (klass->proxy, minuend, subtrahend, &diff, &error))
    {
      /* Method failed, the GError is set, let's warn everyone */
      g_warning ("Woops remote sync add failed: %s", error->message);
      g_error_free (error);
      return;
    }
  g_print ("Sync request: %d - %d = %d\n", minuend, subtrahend, diff);
}

int
main ()
{
  CalcClient *client;
  GMainLoop *loop;

  dbus_g_thread_init ();
  loop = g_main_loop_new (NULL, FALSE);
  client = g_object_new (TYPE_CALC_CLIENT, NULL);
  client_add_async (client, 1, 2);
  client_add_sync (client, 3, 4);
  client_sub_async (client, 8, 7);
  client_sub_sync (client, 9, 2);
  g_main_loop_run (loop);
  return 0;
}
