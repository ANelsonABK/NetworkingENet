//#pragma once
//#include <vector>
//#include <math.h>
//
//using namespace std;
//
//constexpr auto RPC_NUM_TYPES = 11;
//
//const int AnySize[RPC_NUM_TYPES] = {
//	1, 1,
//	sizeof(char), sizeof(unsigned char), sizeof(short),
//	sizeof(unsigned short), sizeof(int), sizeof(unsigned int),
//	sizeof(double), sizeof(float),
//	//sizeof(Vector2), sizeof(vec3), sizeof(vec4), sizeof(quat),
//	//sizeof(mat2), sizeof(mat3), sizeof(mat4),
//	1
//};
//
//const string AnyStr[RPC_NUM_TYPES] = {
//	"String", "Vector",
//	"Char", "UChar", "Short",
//	"UShort", "Int", "UInt",
//	"Double", "Float",
//	//"Vec2", "Vec3", "Vec4", "Quat",
//	//"Mat2", "Mat3", "Mat4", 
//	"Map"
//};
//
//typedef vector<u_char> Message;
//
//struct Any
//{
//	enum Type {
//		String = 0, Vector = 1,
//		Char = 2, UChar = 3, Short = 4,
//		UShort = 5, Int = 6, UInt = 7,
//		Double = 8, Float = 9,
//		//Vec2 = 10, Vec3 = 11, Vec4 = 12, Quat = 13,
//		//Mat2 = 14, Mat3 = 15, Mat4 = 16, 
//		Map = 11
//		// >=32 : num = type-32
//	};
//
//	union All
//	{
//		double d; float f; char c;
//		short s; u_short us;
//		int i; u_int ui;
//		u_int  u64[2];
//		u_char uc[16];
//	};
//
//	inline void CheckDoubleByteOrder(All& a)
//	{
//		/*
//		* htons converts a u_short from host to
//		* TCP/IP network byte order (which is 
//		* big-endian).
//		*/
//		if (htons(1) == 1) return;
//
//		u_int b = a.u64[0];
//		a.u64[0] = a.u64[1];
//		a.u64[1] = b;
//
//		return;
//	}
//
//	// Constructors
//	Any() { _type = 0; }
//
//	Any(double e)
//	{
//		_num = e; _type = Double;
//		if (e - (double)((u_char)e) == 0) { _type = UChar; return; }
//		if (e - (double)((char)e) == 0) { _type = Char; return; }
//		if (e - (double)((short)e) == 0) { _type = Short; return; }
//		if (e - (double)((u_short)e) == 0) { _type = UShort; return; }
//		if (e - (double)((int)e) == 0) { _type = Int; return; }
//		if (e - (double)((u_int)e) == 0) { _type = UInt; return; }
//		if (e - (double)((float)e) == 0) { _type = Float; return; }
//	}
//
//	Any(string e) { _str = e; _type = String; }
//	//Any(vec2 v) { _f[0] = v.x; _f[1] = v.y;  _type = Vec2; };
//	//Any(vec3 v) { _f[0] = v.x; _f[1] = v.y; _f[2] = v.z;  _type = Vec3; };
//	//Any(vec4 v) { _f[0] = v.x; _f[1] = v.y; _f[2] = v.z; _f[3] = v.w;  _type = Vec4; };
//	//Any(quat v) { _f[0] = v.x; _f[1] = v.y; _f[2] = v.z; _f[3] = v.w;  _type = Quat; };
//	//Any(mat2 m) { _type = Mat2; loopi(0, 2)loopj(0, 2)_f[j * 2 + i] = m[j][i]; };
//	//Any(mat3 m) { _type = Mat3; loopi(0, 3)loopj(0, 3)_f[j * 3 + i] = m[j][i]; };
//	//Any(mat4 m) { _type = Mat4; loopi(0, 4)loopj(0, 4)_f[j * 4 + i] = m[j][i]; };
//	template<class T, class U>
//	Any(map<T, U> m)
//	{
//		_vec.clear(); _vec2.clear();
//		for (auto it = m.begin(); it != m.end(); it++) {
//			Any first(it->first);
//			Any second(it->second);
//			_vec.push_back(first);
//			_vec2.push_back(second);
//		}
//		_type = Map;
//	};
//
//	template<class T>
//	Any(vector<T> v)
//	{
//		_vec.clear();
//		loopi(0, v.size()) _vec.push_back(v[i]);
//		_type = Vector;
//	};
//
//	// Functions
//	int GetType() const { return _type; }
//	string GetTypeString() { return AnyStr[_type]; }
//	string GetTypeString(int type) { return AnyStr[_type]; }
//
//	string GetDataAsString()
//	{
//		ostringstream str{};
//		switch (_type)
//		{
//		    case Char:
//		    case UChar:
//		    case Short:
//		    case UShort:
//		    case Int:
//		    case UInt:
//		    {
//			    str << (int)_num;
//			    break;
//		    }
//			case Float:
//			{
//				str << (float)_num;
//				break;
//			}
//			case Double:
//			{
//				str << (float)_num;
//				break;
//			}
//			case String:
//			{
//				str << _str;
//				break;
//			}
//			case Vector:
//			{
//				str << "[ ";
//				for (auto i : _vec)
//				{
//					str << i.GetDataAsString() << " ";
//				}
//				str << " ";
//				break;
//			}
//			case Map:
//			{
//				str << "[ ";
//				// loop
//				str << "]";
//				break;
//			}
//		}
//
//		return str.str();
//	}
//
//	void Push(vector<u_char>& n)
//	{
//		All a;
//		int size = 0;
//
//		n.push_back((int)_type);
//
//		switch (_type)
//		{
//		    case UChar:
//			{
//				if ((u_char)_num < (256 - 32))
//				{
//					n[n.size() - 1] = ((u_char)_num) + 32;
//					return;
//				}
//
//				a.uc[0] = (u_char)_num;
//				size = sizeof(u_char);
//				break;
//			}
//			case Char:
//			{
//				a.c = (char)_num;
//				size = sizeof(char);
//				break;
//			}
//			case Short:
//			{
//				a.s = (short)_num;
//				a.us = htons(a.us);
//				size = sizeof(short);
//				break;
//			}
//			case UShort:
//			{
//				a.us = (u_short)_num;
//				a.us = htons(a.us);
//				size = sizeof(u_short);
//				break;
//			}
//			case Int:
//			{
//				a.i = (int)_num;
//				a.ui = htons(a.ui);
//				size = sizeof(int);
//				break;
//			}
//			case UInt:
//			{
//				a.ui = (u_int)_num;
//				a.ui = htons(a.ui);
//				size = sizeof(u_int);
//				break;
//			}
//			case Float:
//			{
//a.f = (float)_num;
//a.ui = htons(a.ui);
//size = sizeof(char);
//break;
//			}
//			case Double:
//			{
//				a.d = (double)_num;
//				a.ui = htons(a.ui);
//				size = sizeof(double);
//				break;
//			}
//		}
//	}
//
//	/* Check types */
//	bool IsNumber() { return (_type >= Char || _type <= Float) ? true : false; }
//	bool IsNumber(int t) { return (t >= Char || t <= Float) ? true : false; }
//	bool IsType(u_int type)
//	{
//		return (IsNumber() != IsNumber(type) ||
//			!IsNumber() && type != type) ? false : true;
//	}
//
//	int Pop(vector<u_char>& n, int index = 0)
//	{
//		_type = UChar;
//
//		if (index < 0) { return -1; }
//		if (index >= (int)n.size()) { return -1; }
//
//		All a;
//		int size = 0;
//		_type = n[index];
//		index++;
//
//		if(_type >= RPC_NUM_TYPES) 
//		{
//			_num = _type - 32;
//			_type = UChar;
//			return index;
//		}
//
//		if (index >= (int)n.size()) { return -1; }
//	}
//
//	/* Get Types */
//	bool GetT(u_char& a) { a = (u_char)_num; return IsType(UChar); };
//	bool GetT(char& a) { a = (char)_num; return IsType(Char); };
//
//	bool GetT(short& a) { a = (short)_num; return IsType(Short); };
//	bool GetT(u_short& a) { a = (u_short)_num; return IsType(UShort); };
//
//	bool GetT(int& a) { a = (int)_num; return IsType(Int); };
//	bool GetT(u_int& a) { a = (u_int)_num; return IsType(UInt); };
//
//	bool GetT(float& a) { a = (float)_num; return IsType(Float); };
//	bool GetT(double& a) { a = (double)_num; return IsType(Double); };
//
//	bool GetT(string& a) { a = _str; return IsType(String); };
//
//	template<class T>
//	bool GetT(vector<T>& v)
//	{
//		if (!IsType(Vector)) { return false; }
//
//		v.clear();
//
//		// loop
//
//		return true;
//	}
//
//	template<class T, class U>
//	bool GetT(map<T, U>& m)
//	{
//		if (!IsType(Map)) { return false; }
//
//		map.clear();
//
//		// loop
//
//		return true;
//	};
//
//	int GetType() { return _type; }
//
//private:
//	u_char  _type;
//	string _str;
//	double _num;
//	float _f[16];
//	vector<Any> _vec, _vec2; // vec2 is used for map
//};
//
//class Rpc
//{
//public:
//	// Constructor
//	Rpc() { };
//
//	/* Utility */
//	template<int...> struct IndexTuple {};
//
//	template<int I, typename IndexTuple, typename... Types>
//	struct MakeIndexesImpl;
//
//	template<int I, int... Indexes, typename T, typename... Types>
//	struct MakeIndexesImpl <I, IndexTuple<Indexes...>, T, Types...>
//	{
//		typedef typename MakeIndexesImpl<I + 1,
//			IndexTuple<Indexes..., I>, Types...>::type type;
//	};
//
//	template<int I, int... Indexes>
//	struct MakeIndexesImpl <I, IndexTuple<Indexes...> >
//	{
//		typedef IndexTuple<Indexes...> type;
//	};
//
//	template<typename... Types>
//	struct MakeIndexes : MakeIndexesImpl < 0, IndexTuple<>, Types... > {};
//
//	template<typename T> T static ReadFrom(int index, vector<Any> &list)
//	{
//		T t;
//		list[index].GetT(t);
//		return t;
//	}
//
//	template <typename T> bool static VerifyArg(int index, vector<Any>& list)
//	{
//		T t;
//		bool isType = list[index].GetT(t);
//		if (!isType)
//		{
//			cout << "Error for parameter " << index << endl;
//		}
//
//		return isType;
//	}
//
//	/* RPC Data to params */
//	vector< function<void(vector<u_char>&, int&, string&, u_int)> > _functionArray;
//
//	template<typename... Args, class F, int... Is>
//	void RegisterRPCIndex(string name, F f, IndexTuple<Is...>)
//	{
//		const int n = sizeof...(Args);
//
//		_localFunctions.push_back(name);
//
//		_functionArray.push_back([f](vector<u_char>& data, int& index,
//			string& name, u_int hostid)
//			{
//				const int n = sizeof...(Args);
//
//				vector<Any> list;
//				int count = n;
//				bool server = (hostid != 0xffffffff);
//
//				// Server rpcs have the client id as first parameter
//				if (server) {
//					list.push_back(hostid);
//					count--;
//				}
//
//				if (index >= (int)data.size())
//				{
//					index = -1;
//					return;
//				}
//
//				Any anyArgs;
//				int numArgs;
//				index = anyArgs.Pop(data, index);
//
//				if (index == -1) { return; }
//
//				anyArgs.GetT(numArgs);
//
//				if (count != numArgs)
//				{
//					cout << " RPC " << name << " arg count mismatch "
//						<< count << " != " << numArgs << endl;
//					index = -1;
//					return;
//				}
//
//				for (int i = 0; i < numArgs; i++)
//				{
//					Any a;
//					if (index >= (int)data.size())
//					{
//						index = -1;
//						return;
//					}
//
//					index = a.Pop(data, index);
//					if (index < 0) { return; }
//
//					list.push_back(a);
//				}
//
//				vector<bool> isType = { VerifyArg<Args>(Is, list)... };
//
//				// check if there is a type mismatch
//				for (auto i : isType) { if (!i) return; }
//
//				f(ReadFrom<Args>(Is, list)...);
//			});
//	}
//};