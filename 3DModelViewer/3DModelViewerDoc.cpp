
// 3DModelViewerDoc.cpp : implementation of the CMy3DModelViewerDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "3DModelViewer.h"
#endif

#include "3DModelViewerDoc.h"

#include <propkey.h>
#include "miniz.h"
#include "ObjLoader.h"
#include "tlC3DGfx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMy3DModelViewerDoc

IMPLEMENT_DYNCREATE(CMy3DModelViewerDoc, CDocument)

BEGIN_MESSAGE_MAP(CMy3DModelViewerDoc, CDocument)
END_MESSAGE_MAP()


// CMy3DModelViewerDoc construction/destruction

CMy3DModelViewerDoc::CMy3DModelViewerDoc()
{
	// TODO: add one-time construction code here

}

CMy3DModelViewerDoc::~CMy3DModelViewerDoc()
{
}

BOOL CMy3DModelViewerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CMy3DModelViewerDoc serialization

void CMy3DModelViewerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CMy3DModelViewerDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CMy3DModelViewerDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CMy3DModelViewerDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CMy3DModelViewerDoc diagnostics

#ifdef _DEBUG
void CMy3DModelViewerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMy3DModelViewerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMy3DModelViewerDoc commands


BOOL CMy3DModelViewerDoc::OnOpenDocument ( LPCTSTR lpszPathName )
{
	if ( !CDocument::OnOpenDocument ( lpszPathName ) )
		return FALSE;

	// TODO:  Add your specialized creation code here
	FILE* pFile ;
	_wfopen_s ( &pFile, lpszPathName, L"rb" ) ;
	if ( ! pFile )
		return FALSE ;

	fseek ( pFile, 0, SEEK_END ) ;
	int iSize = ftell ( pFile ) ;
	fseek ( pFile, 0, SEEK_SET ) ;

	char* pBuf = new char [ iSize ] ;
	fread ( pBuf, iSize, 1, pFile ) ;
	fclose ( pFile ) ;

	LoadModelFromMemory ( pBuf, iSize ) ;
	delete pBuf ;

	return TRUE;
}

bool CMy3DModelViewerDoc::LoadModelFromMemory ( void* pData, DWORD dwDataSize )
{
	if ( ! pData )
		return false;

	mz_zip_archive archive;

	ZeroMemory ( &archive, sizeof ( archive ) );

	if ( !mz_zip_reader_init_mem ( &archive, pData, dwDataSize, 0 ) )
		return false;

	if ( 0 ) {
		size_t iDecompressed = 0;
		void* pDecompressedData = mz_zip_reader_extract_file_to_heap ( &archive, "header.json", &iDecompressed, 0 );

		// TODO: Find a better way if any
		char* pszJsonText = new char [ iDecompressed + 1 ];
		assert ( pszJsonText != NULL );
		memcpy_s ( pszJsonText, iDecompressed, pDecompressedData, iDecompressed );
		pszJsonText [ iDecompressed ] = 0;
		delete pDecompressedData;

		//LoadobjectFromJson ( pszJsonText, pObjectSlot );
		delete pszJsonText;
	}

	mz_zip_reader_end ( &archive );

	// 		char szSystemTempPath [ MAX_PATH ] ;
	// 		GetTempPathA ( MAX_PATH, szSystemTempPath ) ;
	// 		strcpy_s ( szSystemTempPath, MAX_PATH, ".\\Temp\\" ) ;

	char szSystemTempPath [ MAX_PATH ];
	if ( 0 == GetTempPathA ( MAX_PATH, szSystemTempPath ) ) {
	strcpy_s ( szSystemTempPath, MAX_PATH, "." );
	}

	char szTempName [ MAX_PATH ];
	if ( 0 == GetTempFileNameA ( NULL, NULL, 0, szTempName ) ) {
	strcpy_s ( szTempName, MAX_PATH, "\\tmp" );
	}

	char szPath [ MAX_PATH ];
	strcpy_s ( szPath, MAX_PATH, szSystemTempPath );
	strcat_s ( szPath, MAX_PATH, szTempName );

	BOOL bResult = CreateDirectoryA ( szPath, NULL );

	strcat_s ( szPath, MAX_PATH, "\\" );

	if ( !bResult ) {
		strcpy_s ( szPath, MAX_PATH, ".\\tmp\\" );
		}

		ZeroMemory ( &archive, sizeof ( archive ) );

		if ( !mz_zip_reader_init_mem ( &archive, pData, dwDataSize, 0 ) )
		return false;

		int iFileCount = mz_zip_reader_get_num_files ( &archive );

		char szMesh [ MAX_PATH ] ;
		for ( int iFile = 0; iFile < iFileCount; iFile++ ) {

			char szSrcFile [ MAX_PATH ];
			mz_zip_reader_get_filename ( &archive, iFile, szSrcFile, MAX_PATH );

			char szDestFile [ MAX_PATH ];
			strcpy_s ( szDestFile, MAX_PATH, szPath );
			strcat_s ( szDestFile, MAX_PATH, szSrcFile );

			if ( strstr ( szSrcFile, ".obj" ) != 0 ) {
				strcpy ( szMesh, szSrcFile ) ;
			}

			mz_zip_reader_extract_file_to_file ( &archive, szSrcFile, szDestFile, 0 );
		}

		mz_zip_reader_end ( &archive );

		char szMeshFilename [ MAX_PATH ];
		strcpy_s ( szMeshFilename, MAX_PATH, szPath );
		//strcat_s ( szMeshFilename, MAX_PATH, pObjectSlot->szMeshFilename );
		strcat_s ( szMeshFilename, MAX_PATH, szMesh );

		wchar_t wszMeshFilename [ MAX_PATH ];
		int iLen = MultiByteToWideChar ( CP_ACP, 0, szMeshFilename, strlen ( szMeshFilename ), wszMeshFilename, MAX_PATH ) ;
		wszMeshFilename [ iLen ] = 0 ;

		TObjMesh mesh ;
		int iRes = LoadObj ( wszMeshFilename, &mesh ) ;

		SetCurrentDirectoryA ( szPath ) ;

		m_Mesh.Create ( C3DGfx::GetInstance ()->GetDevice (), mesh, FALSE, TRUE ) ;

		//bool bRes = CXFileUtil::LoadXFile ( C3DGfx::Instance ()->GetDevice (),
		//szMeshFilename,
		//szPath,
		//"Data/Shader/",
		//&pObjectSlot->pMesh );


		{ // Clean temp files
			char szFind [ MAX_PATH ];
			strcpy_s ( szFind, MAX_PATH, szPath );
			strcat_s ( szFind, "*.*" );

			WIN32_FIND_DATAA fd;
			HANDLE hFind = FindFirstFileA ( szFind, &fd );
			while ( FindNextFileA ( hFind, &fd ) ) {
			char szFile [ MAX_PATH ];
			strcpy_s ( szFile, MAX_PATH, szPath );
			strcat_s ( szFile, fd.cFileName );

			BOOL bb = DeleteFileA ( szFile );
			bb = 0;
		}
		FindClose ( hFind );

		RemoveDirectoryA ( szPath );
	}

	return true;
}