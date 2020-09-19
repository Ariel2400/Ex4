class ClientHandler {
    public:
        virtual void handleClient(InputStream is, OutputStream os) = 0;
}