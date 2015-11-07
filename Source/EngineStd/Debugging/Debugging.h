#ifndef DEBUGGING_H
#define DEBUGGING_H

#if !defined(SAFE_DELETE)
	#define SAFE_DELETE(x) if(x) delete x; x= nullptr;
#endif

#if !defined(SAFE_DELETE_ARRAY)
	#define SAFE_DELETE_ARRAY(x) if (x) delete [] x; x = nullptr;
#endif

#if !defined(SAFE_RELEASE)
	#define SAFE_RELEASE(x) if(x) x->Release(); x = nullptr;
#endif

#endif // DEBUGGING_H
