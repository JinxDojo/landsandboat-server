-- NOTE: This file is autogenerated!
-- You can (and should) change the values in here to whatever you desire.
-- If you want to add new settings, go look at tools/generate_settings.py

xi = xi or {}
xi.settings = xi.settings or {}

xi.settings.zmq =
{
    SERVER_IP = "127.0.0.1", -- (string) The IP of the machine ZMQ operates on. THIS SHOULD BE YOUR LOCAL MACHINE!
    SERVER_PORT = 54003, -- (uint) The port ZMQ operates on (inter-process messaging)
}