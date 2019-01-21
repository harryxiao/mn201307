package com.goodix.device;

public class MessageType {
	public static final int MSG_TYPE_COMMON_BASE = 0x00000000;
	public static final int MSG_TYPE_COMMON_TOUCH = MSG_TYPE_COMMON_BASE + 1;
	public static final int MSG_TYPE_COMMON_UNTOUCH = MSG_TYPE_COMMON_BASE + 2;
	public static final int MSG_TYPE_COMMON_NOTIFY_INFO = MSG_TYPE_COMMON_BASE + 7;

	public static final int MSG_TYPE_REGISTER_BASE = 0x00000010;
	public static final int MSG_TYPE_REGISTER_PIECE = MSG_TYPE_REGISTER_BASE + 1;
	public static final int MSG_TYPE_REGISTER_NO_PIECE = MSG_TYPE_REGISTER_BASE + 2;
	public static final int MSG_TYPE_REGISTER_NO_EXTRAINFO = MSG_TYPE_REGISTER_BASE + 3;
	public static final int MSG_TYPE_REGISTER_LOW_COVER = MSG_TYPE_REGISTER_BASE + 4;
	public static final int MSG_TYPE_REGISTER_BAD_IMAGE = MSG_TYPE_REGISTER_BASE + 5;
	public static final int MSG_TYPE_REGISTER_GET_DATA_FAILED = MSG_TYPE_REGISTER_BASE + 6;
	public static final int MSG_TYPE_REGISTER_TIMEOUT = MSG_TYPE_REGISTER_BASE + 7;
	public static final int MSG_TYPE_REGISTER_COMPLETE = MSG_TYPE_REGISTER_BASE + 8;
	public static final int MSG_TYPE_REGISTER_CANCEL = MSG_TYPE_REGISTER_BASE + 9;
	public static final int MSG_TYPE_REGISTER_DUPLICATE_REG = MSG_TYPE_REGISTER_BASE + 10;

	public static final int MSG_TYPE_RECONGNIZE_BASE = 0x00000100;
	public static final int MSG_TYPE_RECONGNIZE_SUCCESS = MSG_TYPE_RECONGNIZE_BASE + 1;
	public static final int MSG_TYPE_RECONGNIZE_TIMEOUT = MSG_TYPE_RECONGNIZE_BASE + 2;
	public static final int MSG_TYPE_RECONGNIZE_FAILED = MSG_TYPE_RECONGNIZE_BASE + 3;
	public static final int MSG_TYPE_RECONGNIZE_BAD_IMAGE = MSG_TYPE_RECONGNIZE_BASE + 4;
	public static final int MSG_TYPE_RECONGNIZE_GET_DATA_FAILED = MSG_TYPE_RECONGNIZE_BASE + 5;
	public static final int MSG_TYPE_RECONGNIZE_NO_REGISTER_DATA = MSG_TYPE_RECONGNIZE_BASE + 6;// new

	public static final int MSG_TYPE_DELETE_BASE = 0x00001000;
	public static final int MSG_TYPE_DELETE_SUCCESS = MSG_TYPE_DELETE_BASE + 1;
	public static final int MSG_TYPE_DELETE_NOEXIST = MSG_TYPE_DELETE_BASE + 2;
	public static final int MSG_TYPE_DELETE_TIMEOUT = MSG_TYPE_DELETE_BASE + 3;

	public static final int MSG_TYPE_ERROR = 0x00010000;

	public static String getString(int msg) {
		switch (msg) {
		case MSG_TYPE_COMMON_BASE:
			return "MSG_TYPE_COMMON_BASE";
		case MSG_TYPE_COMMON_TOUCH:
			return "TOUCH";
		case MSG_TYPE_COMMON_UNTOUCH:
			return "UNTOUCH";
		case MSG_TYPE_COMMON_NOTIFY_INFO:
			return "MSG_TYPE_COMMON_NOTIFY_INFO";
		case MSG_TYPE_REGISTER_BASE:
			return "MSG_TYPE_REGISTER_BASE";
		case MSG_TYPE_REGISTER_PIECE:
			return "MSG_TYPE_REGISTER_PIECE";
		case MSG_TYPE_REGISTER_NO_PIECE:
			return "MSG_TYPE_REGISTER_NO_PIECE";
		case MSG_TYPE_REGISTER_NO_EXTRAINFO:
			return "MSG_TYPE_REGISTER_NO_EXTRAINFO";
		case MSG_TYPE_REGISTER_LOW_COVER:
			return "MSG_TYPE_REGISTER_LOW_COVER";
		case MSG_TYPE_REGISTER_BAD_IMAGE:
			return "MSG_TYPE_REGISTER_BAD_IMAGE";
		case MSG_TYPE_REGISTER_GET_DATA_FAILED:
			return "MSG_TYPE_REGISTER_GET_DATA_FAILED";
		case MSG_TYPE_REGISTER_TIMEOUT:
			return "MSG_TYPE_REGISTER_TIMEOUT";
		case MSG_TYPE_REGISTER_COMPLETE:
			return "MSG_TYPE_REGISTER_COMPLETE";
		case MSG_TYPE_REGISTER_CANCEL:
			return "MSG_TYPE_REGISTER_CANCEL";
		case MSG_TYPE_RECONGNIZE_BASE:
			return "MSG_TYPE_RECONGNIZE_BASE";
		case MSG_TYPE_RECONGNIZE_SUCCESS:
			return "SUCCESS";
		case MSG_TYPE_RECONGNIZE_TIMEOUT:
			return "TIMEOUT";
		case MSG_TYPE_RECONGNIZE_FAILED:
			return "FAILED";
		case MSG_TYPE_RECONGNIZE_BAD_IMAGE:
			return "BAD_IMAGE";
		case MSG_TYPE_RECONGNIZE_GET_DATA_FAILED:
			return "GET_DATA_FAILED";
		case MSG_TYPE_RECONGNIZE_NO_REGISTER_DATA:
			return "NO_REGISTER_DATA";
		case MSG_TYPE_DELETE_BASE:
			return "MSG_TYPE_DELETE_BASE";
		case MSG_TYPE_DELETE_SUCCESS:
			return "MSG_TYPE_DELETE_SUCCESS";
		case MSG_TYPE_DELETE_NOEXIST:
			return "MSG_TYPE_DELETE_NOEXIST";
		case MSG_TYPE_DELETE_TIMEOUT:
			return "MSG_TYPE_DELETE_TIMEOUT";
		case MSG_TYPE_ERROR:
			return "MSG_TYPE_ERROR";
		default:
			return "UNKNOWN Message : " + msg;
		}
	}
}