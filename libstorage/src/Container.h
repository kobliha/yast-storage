#ifndef CONTAINER_H
#define CONTAINER_H

#include <list>

using namespace std;

#include "y2storage/Volume.h"
#include "y2storage/StorageTmpl.h"
#include "y2storage/FilterIterator.h"
#include "y2storage/DerefIterator.h"
#include "y2storage/IterPair.h"

template<typename L1, typename L2> class ListListIterator;

class Container
    {
    friend class Storage;
    template<typename L1, typename L2> friend class ListListIterator;

    typedef list<Volume*> VCont;
    typedef VCont::iterator VIter;
    typedef VCont::const_iterator CVIter;

    public:
	struct SkipDeleted 
	    { 
	    bool operator()(const Volume&d) const { return( !d.Delete());}
	    };
	static SkipDeleted SkipDel;
	static bool NotDeleted( const Volume&d ) { return( !d.Delete() ); }
	typedef enum { UNKNOWN, DISK, MD, LOOP, LVM, EVMS } CType;

	template< class Pred >
	    struct CVP { typedef ContainerIter<Pred, CVIter> type; };
	template< class Pred >
	    struct CV { typedef ContainerDerIter<Pred, typename CVP<Pred>::type, const Volume> type; };

	template< class Pred >
	    struct VP { typedef ContainerIter<Pred, VIter> type; };
	template< class Pred >
	    struct V { typedef ContainerDerIter<Pred, typename VP<Pred>::type, Volume> type; };
	template< class Pred >
	    struct VolCondIPair { typedef MakeCondIterPair<Pred, typename CV<Pred>::type> type;};
	    
	typedef CheckFnc<const Volume> CheckFncVol;

	typedef CheckerIterator< CheckFncVol, CVP<CheckFncVol>::type, 
	                         CVIter, Volume> ConstVolPIterator;

	typedef DerefIterator<ConstVolPIterator,const Volume> ConstVolIterator;

	typedef CheckerIterator< CheckFncVol, VP<CheckFncVol>::type, 
	                         VIter, Volume> VolPIterator;
	typedef DerefIterator<VolPIterator,Volume> VolIterator;

	typedef IterPair<ConstVolIterator> ConstVolPair;
	ConstVolPair VolPair( bool (* CheckFnc)( const Volume& )=NULL ) const
	    { 
	    return( ConstVolPair( VolBegin( CheckFnc ), VolEnd( CheckFnc ) ));
	    }
	ConstVolIterator VolBegin( bool (* CheckFnc)( const Volume& )=NULL ) const
	    {
	    return( ConstVolIterator( ConstVolPIterator(Vols.begin(), Vols.end(), CheckFnc )) );
	    }
	ConstVolIterator VolEnd( bool (* CheckFnc)( const Volume& )=NULL ) const
	    {
	    return( ConstVolIterator( ConstVolPIterator(Vols.begin(), Vols.end(), CheckFnc, true )) );
	    }

	template< class Pred > typename VolCondIPair<Pred>::type VolCondPair( const Pred& p ) const
	    {
	    return( VolCondIPair<Pred>::type( VolCondBegin( p ), VolCondEnd( p ) ) );
	    }
	template< class Pred > typename CV<Pred>::type VolCondBegin( const Pred& p ) const
	    {
	    return( CV<Pred>::type( Vols.begin(), Vols.end(), p ) );
	    }
	template< class Pred > typename CV<Pred>::type VolCondEnd( const Pred& p ) const
	    {
	    return( CV<Pred>::type( Vols.begin(), Vols.end(), p, true ) );
	    }

	Container( const string& Name );
	virtual ~Container();
	const string& Name() const { return name; }
	const string& Device() const { return device; }
	CType Type() const { return type; }
	bool Delete() const { return deleted; }
	static CType const StaticType() { return UNKNOWN; } 

    protected:
	typedef CVIter ConstPlainIterator;
	ConstPlainIterator begin() const { return Vols.begin(); }
	ConstPlainIterator end() const { return Vols.end(); }

	typedef VIter PlainIterator;
	PlainIterator begin() { return Vols.begin(); }
	PlainIterator end() { return Vols.end(); }

	typedef IterPair<VolIterator> VolIPair;
	VolIPair VolPair( bool (* CheckFnc)( const Volume& )=NULL )
	    { 
	    return( VolIPair( VBegin( CheckFnc ), VEnd( CheckFnc ) ));
	    }
	VolIterator VBegin( bool (* CheckFnc)( const Volume& )=NULL ) 
	    { 
	    return( VolIterator( VolPIterator(Vols.begin(), Vols.end(), CheckFnc )) );
	    }
	VolIterator VEnd( bool (* CheckFnc)( const Volume& )=NULL ) 
	    { 
	    return( VolIterator( VolPIterator(Vols.begin(), Vols.end(), CheckFnc, true )) );
	    }
	CType type;
	string name;
	string device;
	bool deleted;
	VCont Vols;
    };

#endif
