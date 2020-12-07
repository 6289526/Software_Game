#include"graphic_objmesh.hpp"
#include <math.h>

#ifndef RtlZeroMemory
#define RtlZeroMemory(Destination,Length) memset((Destination),0,(Length))
#endif

#ifndef ZeroMemory
#define ZeroMemory RtlZeroMemory
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x) { if (x) { delete [] (x); (x) = NULL; } }
#endif
//------------------------------------------------------------------------
// Name : GetDirectionPath()
// Desc : 文字列からディレクトリを取得
//------------------------------------------------------------------------
char* GetDirectoryPath( const char* filename )
{
	char *strings = NULL;
	char *dir;
	dir = new char [strlen(filename)+1];
	strcpy(dir, filename);
	if ( strings = strrchr(dir, '/' ) ) strings[1] = '\0';
	else if ( strings = strrchr(dir, '\\') ) strings[1] ='\0';
	else dir[0] = '\0';
	return dir;
}

//-----------------------------------------------------------------------
// Name : CutDirectoryPath()
// Desc : 文字列からディレクトリを削除
//-----------------------------------------------------------------------
char* CutDirectoryPath( const char* filename )
{
	char *s=NULL;
	char *base=NULL;
	base = new char [strlen(filename)+1];
	strcpy( base, filename );
	if ( s = strrchr(base, '\\') )
	{
		s++;
		return s;
	}
	else if ( s = strrchr(base, '/') )
	{
		s++;
		return s;
	}
	else
	{ 
		return base;
	}
}

//-----------------------------------------------------------------------
// Name : SetDirectoryPath
// Desc : ディレクトリを前に付加して文字列を返す
//-----------------------------------------------------------------------
char* SetDirectoryPath( char* dest, char* directory )
{
	char* str;
	str = new char[strlen(directory) + strlen(dest) + 1];
	strcpy( str, directory );
	strcat( str, dest );
	return str;
}

//////////////////////////////////////////////////////////////////////////
// OBJVEC2
//////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------
// Name : OBJVEC2()
// Desc : コンストラクタ
//------------------------------------------------------------------------
OBJVEC2::OBJVEC2( float nx, float ny )
: x(nx), y(ny)
{
}

//-----------------------------------------------------------------------
// operator float*
//-----------------------------------------------------------------------
OBJVEC2::operator float * () 
{ return (float*)&x; }

//-----------------------------------------------------------------------
// operator const float*
//-----------------------------------------------------------------------
OBJVEC2::operator const float *() const 
{ return (const float*)&x; }


//////////////////////////////////////////////////////////////////////////
// OBJVEC3
//////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------
// Name : OBJVEC3()
// Desc : コンストラクタ
//------------------------------------------------------------------------
OBJVEC3::OBJVEC3( float nx, float ny, float nz )
: x(nx), y(ny), z(nz)
{
}

//------------------------------------------------------------------------
// operator float*
//------------------------------------------------------------------------
OBJVEC3::operator float *() 
{ return (float*)&x; }

//------------------------------------------------------------------------
// operator const float*
//------------------------------------------------------------------------
OBJVEC3::operator const float *() const
{ return (const float*)&x; }

/////////////////////////////////////////////////////////////////////////
// OBJMESH
/////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------
// Name : OBJMESH()
// Desc : コンストラクタ
//-----------------------------------------------------------------------
OBJMESH::OBJMESH()
{
	m_NumVertices = 0;
	m_NumIndices = 0;

	m_Vertices = 0;
	m_Indices = 0;
}

//-----------------------------------------------------------------------
// Name : ~OBJMESH()
// Desc : デストラクタ
//-----------------------------------------------------------------------
OBJMESH::~OBJMESH()
{
	Release();
};

//-----------------------------------------------------------------------
// Name : Release()
// Desc : メモリを破棄
//-----------------------------------------------------------------------
void OBJMESH::Release()
{
	SAFE_DELETE_ARRAY( m_Vertices );
	SAFE_DELETE_ARRAY( m_Indices );
	
	m_NumVertices = 0;
	m_NumIndices = 0;
}

//-----------------------------------------------------------------------
// Name : LoadOBJFile()
// Desc : OBJファイルの読み込み
//-----------------------------------------------------------------------
bool OBJMESH::LoadOBJFile(const char *filename)
{
	ifstream file;

	char buf[OBJ_BUFFER_LENGTH] = {0};
	vector<OBJVEC3> positions;
	vector<OBJVEC3> normals;
	vector<OBJVEC2> texcoords;
	vector<OBJVERTEX> t_vertices;
	vector<unsigned int> t_indices;


	unsigned int dwFaceIndex = 0;
	unsigned int dwFaceCount = 0;

	//　ディレクトリを切り取り
	strcpy( m_directoryPath, GetDirectoryPath( filename ) );

	//　ファイルを開く
	file.open( filename, ios::in );

	//　チェック
	if ( !file.is_open() )
	{
		cerr << "Error : ファイルオープンに失敗\n";
		cerr << "File Name : " << filename << endl;
		return false;
	}

	//　ループ
	for( ;; )
	{
		file >> buf;
		if ( !file )
			break;

		//　コメント
		if ( 0 == strcmp( buf, "#" ) )
		{
		}

		//　頂点座標
		else if ( 0 == strcmp( buf, "v" ) )
		{
			float x, y, z;
			file >> x >> y >> z;
			OBJVEC3 v( x * OBJ_MAGNIFICATION, y * OBJ_MAGNIFICATION, z * OBJ_MAGNIFICATION );
			positions.push_back( v );
		}

		//　テクスチャ座標
		else if ( 0 == strcmp( buf, "vt" ) )
		{
			float u, v;
			file >> u >> v;
			texcoords.push_back( OBJVEC2( u, v ) );
		}

		//　法線ベクトル
		else if ( 0 == strcmp( buf, "vn" ) )
		{
			float x, y, z;
			file >> x >> y >> z;
			normals.push_back( OBJVEC3( x, y, z) );
		}

		//　面
		else if ( 0 == strcmp( buf, "f" ) )
		{
			unsigned int iPosition, iTexCoord, iNormal;
			int p[4] = {-1}, t[4] = {-1}, n[4] = {-1};	
			OBJVERTEX vertex;
			dwFaceIndex++;
			dwFaceCount++;
			int count = 0;
			unsigned int index = 0;
		
			//　三角形・四角形のみ対応
			for ( int iFace = 0; iFace < 4; iFace++ )
			{
				count++;	//　頂点数を数える
				ZeroMemory( &vertex, sizeof( OBJVERTEX ) );

				file >> iPosition;
				vertex.position = positions[ iPosition - 1 ];
				p[iFace] = iPosition -1;
				
				if ( '/' == file.peek() )
				{
					file.ignore();

					//　テクスチャ座標インデックス
					if ( '/' != file.peek() )
					{
						file >> iTexCoord;
						vertex.texcoord = texcoords[ iTexCoord - 1 ];
						t[iFace] = iTexCoord -1;
					}

					//　法線ベクトルインデックス
					if ( '/' == file.peek() )
					{
						file.ignore();

						file >> iNormal;
						vertex.normal = normals[ iNormal - 1 ];
						n[iFace] = iNormal -1;
					}
				}

				//　カウントが3未満
				if ( iFace < 3 )
				{
					t_vertices.push_back( vertex );
					index = t_vertices.size()-1;
					t_indices.push_back( index );
				}

				//　次が改行だったら終了
				if ( '\n' == file.peek() )
				{
					break;
				}

			}

			//　四角形ポリゴンの場合，三角形を追加する
			if ( count > 3 )
			{
				//　カウント
				dwFaceIndex++;
				dwFaceCount++;

				//　頂点とインデックスを追加
				for ( int iFace = 1; iFace < 4; iFace++ )
				{
					int j = (iFace+1)%4;
					ZeroMemory( &vertex, sizeof( OBJVERTEX ) );

					if ( p[j] != -1 ) vertex.position = positions[ p[j] ];
					if ( t[j] != -1 ) vertex.texcoord = texcoords[ t[j] ];
					if ( n[j] != -1 ) vertex.normal = normals[ n[j] ];

					t_vertices.push_back( vertex );
					index = t_vertices.size() - 1;
					t_indices.push_back( index );
				}

			}
		}

		//　マテリアルファイル
		else if ( 0 == strcmp( buf, "mtllib" ) )
		{
		}

		//　マテリアル
		else if ( 0 == strcmp( buf, "usemtl" ) )
		{
        }
	}

	//　ファイルを閉じる
	file.close();

	//　頂点データをコピー
	m_NumVertices = t_vertices.size();
	m_Vertices = new OBJVERTEX[ m_NumVertices ];
	for ( unsigned int i = 0; i<m_NumVertices; i++ )
		m_Vertices[i] = t_vertices[i];


	//　インデックスデータをコピー
	m_NumIndices = t_indices.size();
	m_Indices = new unsigned int [ m_NumIndices ];
	for ( unsigned int i = 0; i<m_NumIndices; i++ )
		m_Indices[i] = t_indices[i];


	//　メモリ破棄
	positions.clear();
	normals.clear();
	texcoords.clear();
	t_vertices.clear();
	t_indices.clear();

	//　正常終了
	return true;
}

//-----------------------------------------------------------------------
// Name : LoadFile()
// Desc : メッシュファイルの読み込み
//-----------------------------------------------------------------------
bool OBJMESH::LoadFile( const char* filename )
{
	//　OBJ, MTLファイルを読み込み
	if ( !LoadOBJFile( filename ) )
	{
		cerr << "Error : メッシュファイルの読み込みに失敗しました\n";
		return false;
	}

	//　正常終了
	printf("complete load objfile\n");
	return true;
}

//-----------------------------------------------------------------------
// Name : Draw()
// Desc : 描画処理
//-----------------------------------------------------------------------
void OBJMESH::Draw(const FloatCube *cube, float dir)
{
	OBJVERTEX *Vertices= new OBJVERTEX[ m_NumVertices ];
	FloatPosition center = {cube->x + cube->w/2, cube->y + cube->h/2, cube->z + cube->d/2};

	//　頂点データをコピー
	for ( unsigned int i = 0; i<m_NumVertices; i++ ){
		Vertices[i] = m_Vertices[i];
		Vertices[i].position.x = center.x + sin(dir) * (m_Vertices[i].position.z) + cos(dir) * (m_Vertices[i].position.x);
		Vertices[i].position.y += cube->y;
		Vertices[i].position.z = center.z + cos(dir) * (m_Vertices[i].position.z) - sin(dir) * (m_Vertices[i].position.x);
	}
	//　三角形描画
    glEnableClientState(GL_VERTEX_ARRAY);
	glInterleavedArrays( GL_T2F_N3F_V3F, 0, Vertices );
	glDrawElements( GL_TRIANGLES, m_NumIndices, GL_UNSIGNED_INT, &m_Indices[0] );
	
	SAFE_DELETE_ARRAY(Vertices);
}


//------------------------------------------------------------------------
// Name : GetNumVertices()
// Desc : 頂点数を取得
//------------------------------------------------------------------------
unsigned int OBJMESH::GetNumVertices()
{ return m_NumVertices; }

//-----------------------------------------------------------------------
// Name : GetNumIndices()
// Desc : インデックス数を取得
//-----------------------------------------------------------------------
unsigned int OBJMESH::GetNumIndices()
{ return m_NumIndices; }

//-----------------------------------------------------------------------
// Name : GetIndexData()
// Desc : 指定されたインデックスデータを取得
//-----------------------------------------------------------------------
unsigned int OBJMESH::GetIndexData( unsigned int index )
{ return m_Indices[index]; }

//-----------------------------------------------------------------------
// Name : GetVertex()
// Desc : 指定された頂点データを取得
//-----------------------------------------------------------------------
OBJVERTEX OBJMESH::GetVertex( unsigned int index )
{ return m_Vertices[index]; }

//-----------------------------------------------------------------------
// Name : GetVertices()
// Desc : 頂点データを取得
//-----------------------------------------------------------------------
OBJVERTEX* OBJMESH::GetVertices()
{ return m_Vertices; }

//-----------------------------------------------------------------------
// Name : GetIndices()
// Desc : インデックスデータを取得
//-----------------------------------------------------------------------
unsigned int* OBJMESH::GetIndices()
{ return m_Indices; }
		