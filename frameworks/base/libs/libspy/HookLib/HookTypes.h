#include <DvmDex.h>

// #ifndef u4
// typedef int u4;
// #endif

// #ifndef u2
// typedef short u2;
// #endif

struct Object {
    /* ptr to class object */
    ClassObject*    clazz;

    /*
     * A word containing either a "thin" lock or a "fat" monitor.  See
     * the comments in Sync.c for a description of its layout.
     */
    u4              lock;
};

#define CLASS_FIELD_SLOTS   4


struct ClassObject : Object {
    /* leave space for instance data; we could access fields directly if we
       freeze the definition of java/lang/Class */
    u4              instanceData[CLASS_FIELD_SLOTS];

    /* UTF-8 descriptor for the class; from constant pool, or on heap
       if generated ("[C") */
    char*           descriptor;
    char*           descriptorAlloc;

    /* access flags; low 16 bits are defined by VM spec */
    u4              accessFlags;

    /* VM-unique class serial number, nonzero, set very early */
    u4              serialNumber;

    /* DexFile from which we came; needed to resolve constant pool entries */
    /* (will be NULL for VM-generated, e.g. arrays and primitive classes) */
    DvmDex*         pDvmDex;
};

struct DexProto {
    DexFile* dexFile;
    u4 protoIdx;
};

struct Method {
    /* the class we are a part of */
    ClassObject*    clazz;

    /* access flags; low 16 bits are defined by spec (could be u2?) */
    u4              accessFlags;

    /*
     * For concrete virtual methods, this is the offset of the method
     * in "vtable".
     *
     * For abstract methods in an interface class, this is the offset
     * of the method in "iftable[n]->methodIndexArray".
     */
    u2             methodIndex;

    /*
     * Method bounds; not needed for an abstract method.
     *
     * For a native method, we compute the size of the argument list, and
     * set "insSize" and "registerSize" equal to it.
     */
    u2              registersSize;  /* ins + locals */
    u2              outsSize;
    u2              insSize;

    /* method name, e.g. "<init>" or "eatLunch" */
    const char*     name;

    /*
     * Method prototype descriptor string (return and argument types).
     *
     * TODO: This currently must specify the DexFile as well as the proto_ids
     * index, because generated Proxy classes don't have a DexFile.  We can
     * remove the DexFile* and reduce the size of this struct if we generate
     * a DEX for proxies.
     */
    DexProto        prototype;

    /* short-form method descriptor string */
    const char*     shorty;

    /*
     * The remaining items are not used for abstract or native methods.
     * (JNI is currently hijacking "insns" as a function pointer, set
     * after the first call.  For internal-native this stays null.)
     */

    /* the actual code */
    const u2*       insns;          /* instructions, in memory-mapped .dex */

    /* JNI: cached argument and return-type hints */
    int             jniArgInfo;

}; 
//      * JNI: native method ptr; could be actual function or a JNI bridge.  We
//      * don't currently discriminate between DalvikBridgeFunc and
//      * DalvikNativeFunc; the former takes an argument superset (i.e. two
//      * extra args) which will be ignored.  If necessary we can use
//      * insns==NULL to detect JNI bridge vs. internal native.
     
//     DalvikBridgeFunc nativeFunc;

//     /*
//      * JNI: true if this static non-synchronized native method (that has no
//      * reference arguments) needs a JNIEnv* and jclass/jobject. Libcore
//      * uses this.
//      */
//     bool fastJni;

//     /*
//      * JNI: true if this method has no reference arguments. This lets the JNI
//      * bridge avoid scanning the shorty for direct pointers that need to be
//      * converted to local references.
//      *
//      * TODO: replace this with a list of indexes of the reference arguments.
//      */
//     bool noRef;

//     /*
//      * JNI: true if we should log entry and exit. This is the only way
//      * developers can log the local references that are passed into their code.
//      * Used for debugging JNI problems in third-party code.
//      */
//     bool shouldTrace;

//     /*
//      * Register map data, if available.  This will point into the DEX file
//      * if the data was computed during pre-verification, or into the
//      * linear alloc area if not.
//      */
//     const RegisterMap* registerMap;

//     /* set if method was called during method profiling */
//     bool            inProfile;
// };*/