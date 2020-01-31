#if !defined(__RingBuffer_hdr__)
#define __RingBuffer_hdr__

#include <cassert>
#include <algorithm>
#include <cmath>

/*! \brief implement a circular buffer of type T
*/
template <class T> 
class CRingBuffer
{
public:
    explicit CRingBuffer (int iBufferLengthInSamples) :
        m_iBuffLength(iBufferLengthInSamples),
        read(0),
        write(0),
        empty(true),
        full(false)
    {
        assert(iBufferLengthInSamples > 0);

        // allocate and init
        buffer = new T[iBufferLengthInSamples];
    }

    virtual ~CRingBuffer ()
    {
        // free memory
        delete[] buffer;
        buffer = nullptr;
    }

    /*! add a new value of type T to write index and increment write index
    \param tNewValue the new value
    \return void
    */
    void putPostInc (T tNewValue)
    {
        if (!full) {
            buffer[write] = tNewValue;
            write = (write + 1) % currentBufferLength;
            if (empty)
                empty = false;
            if (read == write)
                full = true;
        }
        else {
            std::cout << "Buffer Full";
        }
    }

    /*! add new values of type T to write index and increment write index
    \param ptNewBuff: new values
    \param iLength: number of values
    \return void
    */
    void putPostInc (const T* ptNewBuff, int iLength)
    {
    }

    /*! add a new value of type T to write index
    \param tNewValue the new value
    \return void
    */
    void put(T tNewValue)
    {
    }

    /*! add new values of type T to write index
    \param ptNewBuff: new values
    \param iLength: number of values
    \return void
    */
    void put(const T* ptNewBuff, int iLength)
    {
    }
    
    /*! return the value at the current read index and increment the read pointer
    \return float the value from the read index
    */
    T getPostInc ()
    {
        return static_cast<T>(-1);
    }

    /*! return the values starting at the current read index and increment the read pointer
    \param ptBuff: pointer to where the values will be written
    \param iLength: number of values
    \return void
    */
    void getPostInc (T* ptBuff, int iLength)
    {
    }

    /*! return the value at the current read index
    \param fOffset: read at offset from read index
    \return float the value from the read index
    */
    T get (float fOffset = 0.f) const
    {
        return static_cast<T>(-1);
    }

    /*! return the values starting at the current read index
    \param ptBuff to where the values will be written
    \param iLength: number of values
    \return void
    */
    void get (T* ptBuff, int iLength) const
    {
    }
    
    /*! set buffer content and indices to 0
    \return void
    */
    void reset ()
    {
    }

    /*! return the current index for writing/put
    \return int
    */
    int getWriteIdx () const
    {
        return -1;
    }

    /*! move the write index to a new position
    \param iNewWriteIdx: new position
    \return void
    */
    void setWriteIdx (int iNewWriteIdx)
    {
    }

    /*! return the current index for reading/get
    \return int
    */
    int getReadIdx () const
    {
        return -1;
    }

    /*! move the read index to a new position
    \param iNewReadIdx: new position
    \return void
    */
    void setReadIdx (int iNewReadIdx)
    {
    }

    /*! returns the number of values currently buffered (note: 0 could also mean the buffer is full!)
    \return int
    */
    int getNumValuesInBuffer () const
    {
        return -1;
    }

    /*! returns the length of the internal buffer
    \return int
    */
    int getLength () const
    {
        return -1;
    }
private:
    CRingBuffer ();
    CRingBuffer(const CRingBuffer& that);

    int m_iBuffLength;              //!< length of the internal buffer
    int currentBufferLength;

    T *buffer;
    int read;
    int write;
    bool empty;
    bool full;
};
#endif // __RingBuffer_hdr__

/*
 *
class AudioRingBuffer {  // Note: Ideally should be placed separately outside of CombFilter folder.
public:
    AudioRingBuffer(int bufferLength, int numChannels);
    ~AudioRingBuffer();
    Error_t insert(float **audioBuffer, int sampleIndex);
    Error_t fetch(float *sampleBuffer);
    Error_t remove();

private:
    int bufferLength;
    int numChannels;
    float **buffer;
    int read;
    int write;
    bool empty;
    bool full;
};
 */