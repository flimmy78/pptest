#include "rtspApi.h"
#include "SlaveActivator.h"

static SlaveActivator* g_rtsp = NULL;


void rtsp_initialize()
{
	if(g_rtsp) return;
	g_rtsp = new SlaveActivator();
}

void rtsp_finalize()
{
	if(g_rtsp)
	{
		delete g_rtsp;
		g_rtsp = NULL;
	}
}

int rtsp_set_server_port(int port)
{
	if(!g_rtsp) return RTSP_ERROR_INVALID_HANDLE;

	if(port < 1 || port > 65535) return RTSP_ERROR_INVALID_VALUE;

	// port = port
}

int rtsp_set_recv_port(int channel, int port)
{
	if(!g_rtsp) return RTSP_ERROR_INVALID_HANDLE;

	if(channel < 1 || channel > 3) return RTSP_ERROR_INVALID_KEY;

	if(port < 1 || port > 65535) return RTSP_ERROR_INVALID_VALUE;

	// udp_port = port
}

int rtsp_set_log_level(int level)
{
	if(!g_rtsp) return RTSP_ERROR_INVALID_HANDLE;
	
	// log_level = level
}

int rtsp_set_log_output(int output)
{
	if(!g_rtsp) return RTSP_ERROR_INVALID_HANDLE;

	// log_output = output;
}

int rtsp_set_log_output(const char* filename)
{
	if(!g_rtsp) return RTSP_ERROR_INVALID_HANDLE;

	// log filename = filename
	return RTSP_ERROR_NONE;
}

int rtsp_start()
{
	if(!g_rtsp) return RTSP_ERROR_INVALID_HANDLE;

	// start rtsp server
}

int rtsp_stop()
{
	if(!g_rtsp) return RTSP_ERROR_INVALID_HANDLE;

	// stop rtsp server
}
