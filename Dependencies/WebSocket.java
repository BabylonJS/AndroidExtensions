import org.java_websocket.client.WebSocketClient;
import org.java_websocket.handshake.ServerHandshake;
import org.java_websocket.framing.CloseFrame;
import java.net.URI;
import java.net.URISyntaxException;

public class WebSocket extends WebSocketClient {
    public WebSocket(String url) throws URISyntaxException
    {
        super(new URI(url));
    }

    @Override
    public void onOpen(ServerHandshake handshakedata)
    {
        this.openCallback();
    }

    @Override
    public void onMessage(String message)
    {
        this.messageCallback(message);
    }

    private boolean errorFired = false;

    @Override
    public void onClose(int code, String reason, boolean remote)
    {
        // Fire errorCallback for abnormal close codes, but only if onError
        // hasn't already fired it (prevents double error reporting).
        if (!errorFired && code != CloseFrame.NORMAL && code != CloseFrame.ABNORMAL_CLOSE && code != CloseFrame.NEVER_CONNECTED)
        {
            this.errorCallback(reason);
        }

        this.closeCallback(code, reason);
    }

    @Override
    public void onError(Exception ex)
    {
        errorFired = true;
        this.errorCallback(ex.getMessage());
    }

    public native void openCallback();
    public native void closeCallback(int code, String reason);
    public native void messageCallback(String message);
    public native void errorCallback(String message);
}
