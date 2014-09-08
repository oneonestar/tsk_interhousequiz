package hk.edu.tsk.tskinter_housequizcontrolpanel;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;

public class SocketService extends Service {
    public SocketService() {
    }

    @Override
    public IBinder onBind(Intent intent) {
        // TODO: Return the communication channel to the service.
        throw new UnsupportedOperationException("Not yet implemented");
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        return START_REDELIVER_INTENT;
    }
}
