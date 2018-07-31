#include "stdafx.h"
#include "esestore.h"
#include <esent.h>

JET_SESID sessionid;
JET_TABLEID tableid;
JET_INSTANCE instance;
JET_DBID dbid;
JET_COLUMNDEF* column;
int columnid;

esestore::esestore()
{
	std::string eseStoreRoot = "D:\\pwnedpwds\\store\\ese\\";
	std::string esedb = "D:\\pwnedpwds\\store\\ese\\ppwdflt.edb";

	JET_ERR result;

	result = JetCreateInstance(&instance, "lithnetppfilter");

	if (result != 0)
	{
		throw new std::exception("JetCreateInstance failed");
	}

	result = JetSetSystemParameter(&instance, 0, JET_paramCircularLog, 1, 0);

	if (result != 0)
	{
		throw new std::exception("JetSetSystemParameter JET_paramCircularLog failed");
	}

	result = JetSetSystemParameter(&instance, 0, JET_paramCreatePathIfNotExist, 1, 0);

	if (result != 0)
	{
		throw new std::exception("JetSetSystemParameter JET_paramCreatePathIfNotExist failed");
	}

	result = JetSetSystemParameter(&instance, 0, JET_paramExceptionAction, JET_ExceptionNone, 0);

	if (result != 0)
	{
		throw new std::exception("JetSetSystemParameter JET_paramExceptionAction failed");
	}

	result = JetSetSystemParameter(&instance, 0, JET_paramLogFilePath, 0, eseStoreRoot.c_str());

	if (result != 0)
	{
		throw new std::exception("JetSetSystemParameter JET_paramLogFilePath failed");
	}

	result = JetSetSystemParameter(&instance, 0, JET_paramSystemPath, 0, eseStoreRoot.c_str());

	if (result != 0)
	{
		throw new std::exception("JetSetSystemParameter JET_paramSystemPath failed");
	}

	result = JetSetSystemParameter(&instance, 0, JET_paramTempPath, 0, eseStoreRoot.c_str());

	if (result != 0)
	{
		throw new std::exception("JetSetSystemParameter JET_paramTempPath failed");
	}

	result = JetInit(&instance);

	if (result != 0)
	{
		throw new std::exception("JetInit failed");
	}

	result = JetBeginSession(instance, &sessionid, NULL, NULL);

	if (result != 0)
	{
		throw new std::exception("JetBeginSession failed");
	}

	result = JetAttachDatabase(sessionid, esedb.c_str(), 0);

	if (result != 0)
	{
		throw new std::exception("JetAttachDatabase failed");
	}

	result = JetOpenDatabase(sessionid, esedb.c_str(), NULL, &dbid, 0);

	if (result != 0)
	{
		throw new std::exception("JetOpenDatabase failed");
	}

	column = new JET_COLUMNDEF();

	result = JetOpenTable(sessionid, dbid, "pwndpwds", NULL, 0, JET_bitTableReadOnly, &tableid);

	if (result != 0)
	{
		throw new std::exception("JetOpenTable failed");
	}

	result = JetGetTableColumnInfo(sessionid, tableid, "sha1", column, sizeof(JET_COLUMNDEF), JET_ColInfoGrbitMinimalInfo);

	if (result != 0)
	{
		throw new std::exception("JetGetTableColumnInfo failed");
	}

	columnid = column->columnid;
}

bool esestore::IsHashInDb(BYTE* hash)
{
	JET_ERR result;

	result = JetSetCurrentIndex(sessionid, tableid, NULL);
	
	if (result != 0)
	{
		throw new std::exception("JetSetCurrentIndex failed");
	}

	result = JetMakeKey(sessionid, tableid, hash, 20, JET_bitNewKey); 
	if (result != 0)
	{
		throw new std::exception("JetMakeKey failed");
	}

	result = JetSeek(sessionid, tableid, JET_bitSeekEQ);

	if (result == JET_errSuccess)
	{
		return true;
	}

	if (result == JET_errRecordNotFound)
	{
		return false;
	}

	throw new std::exception("JetSeek failed");
}

esestore::~esestore()
{
	if (column)
	{
		delete[] column;
	}

	if (tableid)
	{
		JetCloseTable(sessionid, tableid);
	}

	if (sessionid)
	{
		JetEndSession(sessionid, 0);
	}

	if (instance)
	{
		JetTerm(instance);
	}
}