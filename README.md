# Remote Procedure Calls using C++ Delegates
A C++ standards compliant delegate library capable of invoking a callable delegate function on a remote system. 

See CodeProject article at: <a href="https://www.codeproject.com/Articles/5262271/Remote-Procedure-Calls-using-Cplusplus-Delegates"><strong>Remote Procedure Calls using C++ Delegates</strong></a>

## ⚠️ Deprecated Repository

> **Warning:** This repository is no longer maintained. Please use the modern delegate library in link below.

## New Repository 

[DelegateMQ](https://github.com/endurodave/DelegateMQ) - Invoke any C++ callable function synchronously, asynchronously, or on a remote endpoint.

<h2>Introduction</h2>

<p>C++ delegates simplify usage of a publish/subscribe pattern. With delegates, client code anonymously registers a callback function pointer to receive runtime notification. In other languages, delegates are a first class-feature and built into the language. Not so in C++ which leaves developers to create custom libraries to emulate delegates.</p>

<p>Delegates normally support synchronous executions, that is, when invoked, the bound function is executed within the caller&rsquo;s thread of control. A few years ago, I wrote the article entitled &ldquo;<a href="https://www.codeproject.com/Articles/1160934/Asynchronous-Multicast-Delegates-in-Cplusplus">Asynchronous Multicast Delegates in C++</a>&rdquo;. The library offers synchronous and asynchronous function invocations on any callable function. Simply put, the callback function and callback thread is specified during subscriber registration. During notification, the bound callback function is invoked on the subscriber&rsquo;s desired thread of control.</p>

<p>This article explains an extension to my original C++ delegate library: remote delegates. A remote delegate invokes a function (with data) on a remote system. A remote system is defined as an application running on a different CPU separated by a communication link or a program executing within a separate process. To a user, the delegate appears local; however the library invokes the remote function with little effort. Think of remote delegates as a C++ standard compliant remote procedure call (RPC) implemented using C++ delegates.</p>

<p>The features of the remote delegate library are:</p>

<ol>
	<li><strong>Remote Invocation</strong> &ndash; remotely invoke any callable function (up to 5 arguments)</li>
	<li><strong>Any Protocol</strong> &ndash; supports any transmission medium: UDP, TCP, serial, named pipes</li>
	<li><strong>Any Serialization</strong> &ndash; support for object any serialization method: binary, JSON, XML</li>
	<li><strong>Endianness</strong> &ndash; handles different CPU architectures</li>
	<li><strong>Any Compiler</strong> &ndash; standard C++ code for any compiler without weird hacks</li>
	<li><strong>Any OS</strong> &ndash; easy porting to any OS. Win32, POSIX and std::thread ports included</li>
	<li><strong>Any Function</strong> &ndash; invoke any callable function: member, static, or free</li>
	<li><strong>Any Argument Type</strong> &ndash; supports any argument type: value, reference, pointer, pointer to pointer</li>
	<li><strong>Multiple Arguments</strong> &ndash; supports multiple function arguments</li>
</ol>

<p>The remote delegate implementation significantly eases passing data and objects between remote applications. A small amount of platform-specific code is written to tailor the library to a particular OS and communication protocol. After which, the framework handles all of the low-level machinery to safely invoke any function signature on a remote system.</p>

<p>The original asynchronous delegate implementation strived to ease inter-thread communication by invoking functions and passing data between threads using C++ delegates. Remote delegates extend the library to include inter-process and inter-processor communications.</p>

<p>The original article <a href="https://www.codeproject.com/Articles/1160934/Asynchronous-Multicast-Delegates-in-Cplusplus">Asynchronous Multicast Delegates in C++</a> covers all synchronous and asynchronous delegate library features. The focus of this article is the new remote delegate enhancements.</p>

<h2>Delegates Background</h2>

<p>If you&rsquo;re not familiar with a delegate, the concept is quite simple. A delegate can be thought of as a super function pointer. In C++, there&#39;s no pointer type capable of pointing to all the possible function variations: instance member, virtual, const, static, and free (global). A function pointer can&rsquo;t point to instance member functions, and pointers to member functions have all sorts of limitations. However, delegate classes can, in a type-safe way point to any function provided the function signature matches. In short, a delegate points to any function with a matching signature to support anonymous function invocation.</p>

<p>This C++ delegate implementation is full featured and allows calling any function, even instance member functions, with any arguments either synchronously or asynchronously. The delegate library makes binding to and invoking any function a snap. The addition of remote delegates extends the delegate paradigm to include invoking functions executing within a separate context.</p>

<h2>Using the Code</h2>

<p>I&rsquo;ll first present how to use the code, and then get into the implementation details.</p>

<p>The core delegate library is supported by any C++03 or higher compiler. However, all remote delegate examples are written using some C++11 features. Visual Studio/Win32 and Eclipse/POSIX projects and examples are included.&nbsp;</p>

<p>The delegate library is comprised of delegates and delegate containers. A delegate is capable of binding to a single callable function. A multicast delegate container holds one or more delegates in a list to be invoked sequentially. A single cast delegate container holds at most one delegate.</p>

<p>The new remote delegate classes are show below, where X is the number of function arguments in the target function signature.</p>

<ul class="class">
	<li><code>DelegateRemoteSendX&lt;&gt;</code></li>
	<li><code>DelegateFreeRemoteRecvX&lt;&gt;</code></li>
	<li><code>DelegateMemberRemoteRecvX&lt;&gt;</code></li>
</ul>

<p><code>DelegateRemoteSendX&lt;&gt;</code> initiates invoking a remote function executing on a remote system. The sending system creates this object.</p>

<p><code>DelegateFreeRemoteRecvX&lt;&gt;</code> synchronously invokes a local free callback function located on the receiving remote system.</p>

<p><code>DelegateMemberRemoteRecvX&lt;&gt;</code> synchronously invokes a local member callback function located on the receiving remote system.</p>

<p>The remote delegates are capable of being inserted into any existing delegate container.</p>

<ul class="class">
	<li><code>SinglecastDelegateX&lt;&gt;</code></li>
	<li><code>MulticastDelegateX&lt;&gt;</code></li>
	<li><code>MulticastDelegateSafeX&lt;&gt;</code></li>
</ul>

<h3>Send Data Example</h3>

<p>The send application shows how to invoke a remote function with a single <code>RemoteDataPoint&amp;</code> argument. The key points on the code below are:</p>

<ol>
	<li><code>MakeDelegate()</code> creates a send remote delegate.</li>
	<li>Invoke the send remote delegate.</li>
</ol>

<pre lang="c++">
int main(void)
{
    BOOL result = AfxWinInit(GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0);
    ASSERT_TRUE(result == TRUE);

    result = AfxSocketInit(NULL);
    ASSERT_TRUE(result == TRUE);

    UdpDelegateSend::GetInstance().Initialize();

    // Create a stream to hold send data
    stringstream ss(ios::in | ios::out | ios::binary);

    // Create a send remote delegate
    auto sendDataPointDelegate =
        MakeDelegate&lt;RemoteDataPoint&amp;&gt;(UdpDelegateSend::GetInstance(), ss, REMOTE_DATA_POINT_ID);

    cout &lt;&lt; &quot;Press any key to exit program.&quot; &lt;&lt; endl;

    int x = 1;
    int y = 1;
    while (!_kbhit())
    {
        // Send data point to remote system
        RemoteDataPoint dataPoint(x++, y++);
        sendDataPointDelegate(dataPoint);
    }

    return 0;
}</pre>

<p><code>MakeDelegate()</code> is an overloaded function that helps create delegate objects. Normally <code>MakeDelegate()</code> uses template argument deduction to create the correct instance type based on the arguments. However, a sending delegate doesn&rsquo;t bind to a function; the bound function is on the remote. Therefore, when creating a send delegate the function is called using the template function argument types.</p>

<pre lang="c++">
auto sendDataPointDelegate =
    MakeDelegate&lt;RemoteDataPoint&amp;&gt;(UdpDelegateSend::GetInstance(), ss, REMOTE_DATA_POINT_ID);</pre>

<p>The first argument is the transport object. The second argument is outgoing data byte stream. The last argument is a shared ID between the sender and receiver.</p>

<p>The sender invokes the remote delegate using the correct function arguments. The <code>RemoteDataPoint </code>object is serialized and a message sent to the receiver.</p>

<pre lang="c++">
RemoteDataPoint dataPoint(x++, y++);
sendDataPointDelegate(dataPoint);</pre>

<h3>Receive Delegate Example</h3>

<p>The receive application shows how to register for a remote delegate callback. The key points on the code below:</p>

<ol>
	<li><code>MakeDelegate()</code> creates a receive remote delegate.</li>
	<li><code>RecvDataPointCb()</code> is called when the sender invokes the delegate.</li>
</ol>

<pre lang="c++">
static void RecvDataPointCb(RemoteDataPoint&amp; data)
{
    cout &lt;&lt; &quot;RemoteDataPoint: &quot; &lt;&lt; data.GetX() &lt;&lt; &quot; &quot; &lt;&lt; data.GetY() &lt;&lt; endl;
}

int main(void)
{
    BOOL result = AfxWinInit(GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0);
    ASSERT_TRUE(result == TRUE);

    result = AfxSocketInit(NULL);
    ASSERT_TRUE(result == TRUE);

    UdpDelegateRecv::GetInstance().Initialize();

    // Create a receive remote delegate
    auto recvDataPointDelegate = MakeDelegate(&amp;RecvDataPointCb, REMOTE_DATA_POINT_ID);

    cout &lt;&lt; &quot;Press any key to exit program.&quot; &lt;&lt; endl;

    while (!_kbhit())
        Sleep(10);

    return 0;
}</pre>

<p>The receiver creates a delegate using the same ID as the sender.</p>

<pre lang="c++">
// Create a receive remote delegate
auto recvDataPointDelegate = MakeDelegate(&amp;RecvDataPointCb, REMOTE_DATA_POINT_ID);</pre>

<p>The first argument is a pointer to the callback function. The second argument is the shared ID. The delegate library receives the message, deserialized the <code>ReceiveDataPoint</code> object, and invokes the callback function.</p>

<p>Of course, the delegate library supports member function callbacks, in addition to multiple arguments and different argument types.</p>

<h3>SysData Example</h3>

<p>The remote delegates can be inserted into a delegate container, just like any other delegate type. The types of delegates are summarized below:</p>

<ol>
	<li>Synchronous</li>
	<li>Asynchronous</li>
	<li>Asynchronous Blocking</li>
	<li>Remote</li>
</ol>

<p>This example shows how to register for notification using each delegate type. <code>SysData </code>is a simple class that stores the system mode and notifies clients when changed.</p>

<pre lang="c++">
/// @brief SysData stores common data accessible by any system thread. This class
/// is thread-safe.
class SysData
{
public:
    /// Clients register with MulticastDelegateSafe1 to get callbacks when system mode changes
    static MulticastDelegateSafe1&lt;const SystemModeChanged&amp;&gt; SystemModeChangedDelegate;

    /// Get singleton instance of this class
    static SysData&amp; GetInstance();

    /// Sets the system mode and notify registered clients via SystemModeChangedDelegate.
    /// @param[in] systemMode - the new system mode. 
    void SetSystemMode(SystemMode::Type systemMode);    

private:
    SysData();
    ~SysData();

    /// The current system mode data
    SystemMode::Type m_systemMode;

    /// Lock to make the class thread-safe
    LOCK m_lock;
};</pre>

<p>The <code>SystemModeChangedDelegate </code>container is used by subscribers to register. The function callback signature is <code>void (const SystemModeChanged&amp;)</code>.</p>

<pre lang="c++">
static MulticastDelegateSafe1&lt;const SystemModeChanged&amp;&gt; SystemModeChangedDelegate;</pre>

<p>The <code>SystemModeChanged </code>object is the callback argument type.</p>

<pre lang="c++">
/// @brief Structure to hold system mode callback data. 
class SystemModeChanged
{
public:
    SystemModeChanged() : 
        PreviousSystemMode(SystemMode::STARTING), 
        CurrentSystemMode(SystemMode::STARTING)
    {
    }

    SystemMode::Type PreviousSystemMode;
    SystemMode::Type CurrentSystemMode;

    friend std::ostream&amp; operator&lt;&lt; (std::ostream &amp;out, const SystemModeChanged&amp; data);
    friend std::istream&amp; operator&gt;&gt; (std::istream &amp;in, const SystemModeChanged&amp; data);
};</pre>

<p>When someone calls<code> SysData::SetSystemMode()</code> the new mode is saved and all registered subscribers are notified.</p>

<pre lang="c++">
void SysData::SetSystemMode(SystemMode::Type systemMode)
{
    LockGuard lockGuard(&amp;m_lock);

    // Create the callback data
    SystemModeChanged callbackData;
    callbackData.PreviousSystemMode = m_systemMode;
    callbackData.CurrentSystemMode = systemMode;

    // Update the system mode
    m_systemMode = systemMode;

    // Callback all registered subscribers
    if (SystemModeChangedDelegate)
        SystemModeChangedDelegate(callbackData);
}</pre>

<p><code>TestSysData()</code> registers four callbacks with <code>SysData::SystemModeChangedDelegate</code>. Each callback represents a different notification type: remote, synchronous, asynchronous and asynchronous blocking.</p>

<p>Notice that <code>SysData</code> just exposes a generic delegate container. Each anonymous client decides on the how to be notified. Also note how the arguments used in calling <code>MakeDelegate()</code> dictate the delegate type created.</p>

<pre lang="c++">
// Callback function to receive notifications
static void SystemModeChangedCb(const SystemModeChanged&amp; data)
{
    cout &lt;&lt; &quot;SystemModeChangedCb: &quot; &lt;&lt; data.CurrentSystemMode &lt;&lt; &quot; &quot; &lt;&lt; 
             data.PreviousSystemMode &lt;&lt; endl;
}

void TestSysData()
{
    sysDataWorkerThread.CreateThread();

    // Binary stream of send data bytes
    stringstream ss(ios::in | ios::out | ios::binary);

    // Register to send a remote callback
    SysData::SystemModeChangedDelegate +=
        MakeDelegate&lt;const SystemModeChanged&amp;&gt;(UdpDelegateSend::GetInstance(), 
                     ss, REMOTE_SYSTEM_MODE_CHANGED_ID);

    // Create a receive delegate to receive remote callback
    auto recvDataPointDelegate = 
         MakeDelegate(&amp;SystemModeChangedCb, REMOTE_SYSTEM_MODE_CHANGED_ID);

    // Register for synchronous callback
    SysData::SystemModeChangedDelegate += MakeDelegate(&amp;SystemModeChangedCb);

    // Register for asynchronous callback on a worker thread
    SysData::SystemModeChangedDelegate += 
             MakeDelegate(&amp;SystemModeChangedCb, &amp;sysDataWorkerThread);

    // Register for asynchronous blocking callback on a worker thread
    SysData::SystemModeChangedDelegate += 
             MakeDelegate(&amp;SystemModeChangedCb, &amp;sysDataWorkerThread, 5000);

    // Change system mode. All registered subscribers are notified. 
    SysData::GetInstance().SetSystemMode(SystemMode::STARTING);
    SysData::GetInstance().SetSystemMode(SystemMode::NORMAL);

&nbsp; &nbsp; std::this_thread::sleep_for(std::chrono::seconds(1));

    // Unregister all callbacks
    SysData::SystemModeChangedDelegate -= MakeDelegate(&amp;SystemModeChangedCb);
    SysData::SystemModeChangedDelegate -= 
             MakeDelegate(&amp;SystemModeChangedCb, &amp;sysDataWorkerThread);
    SysData::SystemModeChangedDelegate -= 
             MakeDelegate(&amp;SystemModeChangedCb, &amp;sysDataWorkerThread, 5000);
    SysData::SystemModeChangedDelegate -=
        MakeDelegate&lt;const SystemModeChanged&amp;&gt;
             (UdpDelegateSend::GetInstance(), ss, REMOTE_SYSTEM_MODE_CHANGED_ID);

    sysDataWorkerThread.ExitThread();
}</pre>

<h2>Porting Details</h2>

<p>The remote delegate library is abstracted from object serialization and the communication protocol. Generic interfaces are used by the library to perform these actions.</p>

<h3>Serialization</h3>

<p>Each user-defined data type sent as a remote delegate argument must:</p>

<ol>
	<li>Implement a default constructor.</li>
	<li>Overload the insertion and extraction operators to serialize the object (i.e., <code>operator&lt;&lt;</code> and <code>operator&gt;&gt;</code>).</li>
</ol>

<p>The example below binary serializes (i.e., inserts/extracts) a <code>RemoteDataPoint&amp;</code> object.</p>

<pre lang="c++">
struct RemoteDataPoint
{
public:
    RemoteDataPoint(int x, int y) : m_x(x), m_y(y) {}
    RemoteDataPoint() : m_x(0), m_y(0) {}
    int GetX() const { return m_x; }
    int GetY() const { return m_y; }

private:
    int m_y;
    int m_x;

    friend std::ostream&amp; operator&lt;&lt; (std::ostream &amp;out, const RemoteDataPoint&amp; data);
    friend std::istream&amp; operator&gt;&gt; (std::istream &amp;in, RemoteDataPoint&amp; data);
};

std::ostream&amp; operator&lt;&lt; (std::ostream &amp;out, const RemoteDataPoint&amp; data)
{
&nbsp; &nbsp; out &lt;&lt; data.m_x &lt;&lt; std::endl;
&nbsp; &nbsp; out &lt;&lt; data.m_y &lt;&lt; std::endl;
&nbsp; &nbsp; return out;
}
std::istream&amp; operator&gt;&gt; (std::istream &amp;in, RemoteDataPoint&amp; data)
{
&nbsp; &nbsp; in &gt;&gt; data.m_x;
&nbsp; &nbsp; in &gt;&gt; data.m_y;
&nbsp; &nbsp; return in;
}</pre>

<p>A similar <code>RemoteDataPointJson </code>object is serialized with RapidJSON.</p>

<pre lang="c++">
std::ostream&amp; operator&lt;&lt; (std::ostream &amp;out, const RemoteDataPointJson&amp; data)
{
&nbsp; &nbsp; StringBuffer sb;
&nbsp; &nbsp; PrettyWriter&lt;StringBuffer&gt; writer(sb);

&nbsp; &nbsp; // Serialize object using JSON
&nbsp; &nbsp; data.Serialize(writer);

&nbsp; &nbsp; // Add JSON length&nbsp;
&nbsp; &nbsp; out &lt;&lt; sb.GetLength() + 1;&nbsp;

&nbsp; &nbsp; // Add JSON string
&nbsp; &nbsp; out &lt;&lt; sb.GetString();
&nbsp; &nbsp; return out;
}
std::istream&amp; operator&gt;&gt; (std::istream &amp;in, RemoteDataPointJson&amp; data)
{
&nbsp; &nbsp; // Get JSON length
&nbsp; &nbsp; size_t bufLen = 0;
&nbsp; &nbsp; in &gt;&gt; bufLen;

&nbsp; &nbsp; // Allocate storage buffer
&nbsp; &nbsp; char* buf = (char*)malloc(bufLen);

&nbsp; &nbsp; // Copy JSON into buffer&nbsp;
&nbsp; &nbsp; in.rdbuf()-&gt;sgetn(buf, bufLen);

&nbsp; &nbsp; // Parse JSON
&nbsp; &nbsp; Document d;
&nbsp; &nbsp; d.Parse(buf);

&nbsp; &nbsp; // Get JSON values into object variables
&nbsp; &nbsp; data.m_x = d[&quot;m_x&quot;].GetInt();
&nbsp; &nbsp; data.m_y = d[&quot;m_y&quot;].GetInt();

&nbsp; &nbsp; free(buf);
&nbsp; &nbsp; return in;
}</pre>

<p>The serialization and deserialization method employed is up to you. The only requirement is that an input or output stream is used to hold the serialized object.</p>

<p>A <code>std::stringstream</code> is used in the examples. But any class deriving from <code>std::iostream</code> can be utilized.</p>

<h3>Transport</h3>

<p>The remote delegate library uses the <code>IDelegateTransport</code> class to send data to the remote.</p>

<pre lang="c++">
class IDelegateTransport
{
public:
    /// Dispatch a stream of bytes to a remote system. The implementer is responsible
    /// for sending the bytes over a communication link. Once the receiver obtains the 
    /// bytes, the DelegateRemoteInvoker::DelegateInvoke() function must be called to 
    /// execute the callback on the remote system. 
    /// @param[in] s - an outgoing stream to send to the remote CPU.
    virtual void DispatchDelegate(std::iostream&amp; s) = 0;
};</pre>

<p>A sender inherits from <code>IDelegateTransport </code>and implements <code>DispatchDelegate()</code>. The UDP implementation is shown below:</p>

<pre lang="c++">
void UdpDelegateSend::DispatchDelegate(std::iostream&amp; s)
{
    size_t len = (size_t)s.tellp();
    char* sendBuf = (char*)malloc(len);

    // Copy char buffer into heap allocated memory
    s.rdbuf()-&gt;sgetn(sendBuf, len);

    // Send data to remote system using a socket
    int result = m_sendSocket.Send((void*)sendBuf, len, 0);
    ASSERT_TRUE(result == len);

    free(sendBuf);

    // Reset stream positions
    s.seekg(0);
    s.seekp(0);
}</pre>

<p>Similarly, a Windows named pipe sends remote delegates between processes.</p>

<pre lang="c++">
void PipeDelegateSend::DispatchDelegate(std::iostream&amp; s)
{
    size_t len = (size_t)s.tellp();
    char* sendBuf = (char*)malloc(len);

    // Copy char buffer into heap allocated memory
    s.rdbuf()-&gt;sgetn(sendBuf, len);

    // Send message through named pipe
    DWORD sentLen = 0;
    BOOL success = WriteFile(m_hPipe, sendBuf, len, &amp;sentLen, NULL);
    ASSERT_TRUE(success &amp;&amp; sentLen == len);

    free(sendBuf);

    // Reset stream positions
    s.seekg(0);
    s.seekp(0);
}</pre>

<p>A receiver thread obtains data from a UDP socket. The only requirement is that <code>DelegateRemoteInvoker::Invoke()</code> is called with the incoming stream of data.</p>

<pre lang="c++">
unsigned long UdpDelegateRecv::Process(void* parameter)
{  
    MSG msg;
    const int BUF_SIZE = 1024;
    char recvBuf[BUF_SIZE];

    SOCKADDR_IN addr;
    int addrLen = sizeof(addr);

    BOOL success = AfxSocketInit(NULL);
    ASSERT_TRUE(success == TRUE);

    success = m_recvSocket.Create(514, SOCK_DGRAM, NULL);
    ASSERT_TRUE(success);

    m_started = true;

    for (;;)
    {
        // Check for thread exit message
        if (PeekMessage(&amp;msg, NULL, WM_USER_BEGIN, WM_USER_END, PM_REMOVE) != 0)
        {
            switch (msg.message)
            {
            case WM_EXIT_THREAD:
                m_recvSocket.Close();
                return 0;
            }
        }

        // Check for socket receive message
        int recvMsgSize = m_recvSocket.Receive(recvBuf, BUF_SIZE, 0);
        if (recvMsgSize &gt; 0)
        {
            // Copy receive data bytes into a stringstream
            stringstream ss(ios::in | ios::out | ios::binary);
            ss.write(recvBuf, recvMsgSize);

           // Invoke the remote delegate callback function
           DelegateRemoteInvoker::Invoke(ss);
        }
        else
        {
            Sleep(100);
        }
    }

    return 0;
}</pre>

<p>A named pipe is implemented similarly. Notice the same <code>DelegateRemoteInvoker::Invoke()</code> function is called, only in this case the data bytes are obtained from a named pipe and not a UDP socket.</p>

<pre lang="c++">
unsigned long PipeDelegateRecv::Process(void* parameter)
{  
    MSG msg;
    BOOL connected = FALSE;
    char recvBuf[BUF_SIZE];

    for (;;)
    {
        // Check for thread exit message
        if (PeekMessage(&amp;msg, NULL, WM_USER_BEGIN, WM_USER_END, PM_REMOVE) != 0)
        {
            switch (msg.message)
            {
            case WM_EXIT_THREAD:
                CloseHandle(m_hPipe);
                return 0;
            }
        }

        if (!connected)
        {
            // Check if client connected
            connected = ConnectNamedPipe(m_hPipe, NULL) ?
                TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);
        }
        else
        {
            DWORD recvMsgSize = 0;
            BOOL success = ReadFile(m_hPipe, recvBuf, BUF_SIZE, &amp;recvMsgSize, NULL);

            if (success &amp;&amp; recvMsgSize &gt; 0)
            {
                // Copy receive data bytes into a stringstream
                stringstream ss(ios::in | ios::out | ios::binary);
                ss.write(recvBuf, recvMsgSize);

                // Invoke the remote delegate callback function
                DelegateRemoteInvoker::Invoke(ss);
            }
            else
            {
                Sleep(100);
            }
        }
    }

    return 0;
}</pre>

<p>POSIX <code>UdpDelegateSend&nbsp;</code>and <code>UdpDelegateRecv </code>examples are available in the attached source code. &nbsp;</p>

<p>The <code>DelegateRemoteInvoker()</code> library function simply looks up the matching receive delegate instance by ID and calls <code>DelegateInvoke()</code>.</p>

<pre lang="c++">
bool DelegateRemoteInvoker::Invoke(std::istream&amp; s)
{
&nbsp; &nbsp; // Get id from stream
&nbsp; &nbsp; DelegateIdType id;
&nbsp; &nbsp; s &gt;&gt; id;
&nbsp; &nbsp; s.seekg(0);

&nbsp; &nbsp; // Find invoker instance matching the id
&nbsp; &nbsp; std::map&lt;DelegateIdType, DelegateRemoteInvoker*&gt;::iterator it;
&nbsp; &nbsp; {
&nbsp; &nbsp; &nbsp; &nbsp; LockGuard lockGuard(GetLock());
&nbsp; &nbsp; &nbsp; &nbsp; it = GetMap().find(id);
&nbsp; &nbsp; }
&nbsp; &nbsp; if (it != GetMap().end())
&nbsp; &nbsp; {
&nbsp; &nbsp; &nbsp; &nbsp; // Invoke the delegate instance
&nbsp; &nbsp; &nbsp; &nbsp; (*it).second-&gt;DelegateInvoke(s);
&nbsp; &nbsp; &nbsp; &nbsp; return true;
&nbsp; &nbsp; }
&nbsp; &nbsp; else
&nbsp; &nbsp; {
&nbsp; &nbsp; &nbsp; &nbsp; // No delegate found
&nbsp; &nbsp; &nbsp; &nbsp; return false;
&nbsp; &nbsp; }
}</pre>

<p>The transport mechanism is completely user defined. Any communication medium is supported using a small amount of platform specific code.</p>

<h2>Remote Delegate Details</h2>

<p><code>DelegateRemoteSend1&lt;&gt;</code> implements a single parameter send remote delegate.</p>

<pre lang="c++">
/// @brief Send a delegate to invoke a function on a remote system.&nbsp;
template &lt;class Param1&gt;
class DelegateRemoteSend1 : public Delegate1&lt;Param1&gt; {
public:
&nbsp; &nbsp; DelegateRemoteSend1(IDelegateTransport&amp; transport, std::iostream&amp; stream, DelegateIdType id) :&nbsp;
&nbsp; &nbsp; &nbsp; &nbsp; m_transport(transport), m_stream(stream), m_id(id) { }

&nbsp;&nbsp; &nbsp;virtual DelegateRemoteSend1* Clone() const { return new DelegateRemoteSend1(*this); }

&nbsp;&nbsp; &nbsp;/// Invoke the bound delegate function.&nbsp;
&nbsp;&nbsp; &nbsp;virtual void operator()(Param1 p1) {
&nbsp; &nbsp; &nbsp; &nbsp; m_stream &lt;&lt; m_id &lt;&lt; std::ends;
&nbsp; &nbsp; &nbsp; &nbsp; m_stream &lt;&lt; p1 &lt;&lt; std::ends;
&nbsp; &nbsp; &nbsp; &nbsp; m_transport.DispatchDelegate(m_stream);
&nbsp; &nbsp; }

&nbsp;&nbsp; &nbsp;virtual bool operator==(const DelegateBase&amp; rhs) const {
&nbsp;&nbsp; &nbsp;&nbsp;&nbsp; &nbsp;const DelegateRemoteSend1&lt;Param1&gt;* derivedRhs = dynamic_cast&lt;const DelegateRemoteSend1&lt;Param1&gt;*&gt;(&amp;rhs);
&nbsp;&nbsp; &nbsp;&nbsp;&nbsp; &nbsp;return derivedRhs &amp;&amp;
&nbsp;&nbsp; &nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp; &nbsp;m_id == derivedRhs-&gt;m_id &amp;&amp;
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &amp;m_transport == &amp;derivedRhs-&gt;m_transport; }

private:
&nbsp;&nbsp; &nbsp;IDelegateTransport&amp; m_transport; &nbsp; &nbsp;// Object sends data to remote
&nbsp; &nbsp; std::iostream&amp; m_stream; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;// Storage for remote message&nbsp;
&nbsp; &nbsp; DelegateIdType m_id; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;// Remote delegate identifier
};</pre>

<p>The constructor requires a <code>IDelegateTransport&amp;</code>, a <code>std::iostream&amp;</code> and a <code>DelegateIdType</code>.</p>

<p>The <code>operator()</code> inserts <code>m_id</code> into the stream then calls <code>operator&lt;&lt;</code> on each parameter to serialize. After which, <code>DispatchDelegate()</code> is called to send the delegate the remote system.</p>

<p><code>DelegateFreeRemoteRecv1&lt;&gt;</code> implements a single parameter receive remote delegate.</p>

<pre lang="c++">
template &lt;class Param1&gt;
class DelegateFreeRemoteRecv1 : public DelegateFree1&lt;Param1&gt;, public DelegateRemoteInvoker {
public:
&nbsp; &nbsp; typedef void(*FreeFunc)(Param1);

&nbsp; &nbsp; // Contructors take a free function and delegete id&nbsp;
&nbsp; &nbsp; DelegateFreeRemoteRecv1(FreeFunc func, DelegateIdType id) : DelegateRemoteInvoker(id) { Bind(func, id); }

&nbsp; &nbsp; /// Bind a free function to the delegate.
&nbsp; &nbsp; void Bind(FreeFunc func, DelegateIdType id) {
&nbsp; &nbsp; &nbsp; &nbsp; m_id = id;
&nbsp; &nbsp; &nbsp; &nbsp; DelegateFree1&lt;Param1&gt;::Bind(func);
&nbsp; &nbsp; }

&nbsp; &nbsp; virtual DelegateFreeRemoteRecv1* Clone() const { return new DelegateFreeRemoteRecv1(*this); }

&nbsp; &nbsp; /// Called by the remote system to invoke the delegate function
&nbsp; &nbsp; virtual void DelegateInvoke(std::istream&amp; stream) {
&nbsp; &nbsp; &nbsp; &nbsp; RemoteParam&lt;Param1&gt; param1;

&nbsp; &nbsp; &nbsp; &nbsp; Param1 p1 = param1.Get();

&nbsp; &nbsp; &nbsp; &nbsp; stream &gt;&gt; m_id;
&nbsp; &nbsp; &nbsp; &nbsp; stream.seekg(stream.tellg() + std::streampos(1));
&nbsp; &nbsp; &nbsp; &nbsp; stream &gt;&gt; p1;
&nbsp; &nbsp; &nbsp; &nbsp; stream.seekg(stream.tellg() + std::streampos(1));

&nbsp; &nbsp; &nbsp; &nbsp; DelegateFree1&lt;Param1&gt;::operator()(p1);
&nbsp; &nbsp; }

&nbsp; &nbsp; virtual bool operator==(const DelegateBase&amp; rhs) const {
&nbsp; &nbsp; &nbsp; &nbsp; const DelegateFreeRemoteRecv1&lt;Param1&gt;* derivedRhs = dynamic_cast&lt;const DelegateFreeRemoteRecv1&lt;Param1&gt;*&gt;(&amp;rhs);
&nbsp; &nbsp; &nbsp; &nbsp; return derivedRhs &amp;&amp;
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; m_id == derivedRhs-&gt;m_id &amp;&amp;
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; DelegateFree1&lt;Param1&gt;::operator == (rhs);
&nbsp; &nbsp; }

private:
&nbsp; &nbsp; DelegateIdType m_id; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; // Remote delegate identifier
};</pre>

<p>The constructor takes a <code>FreeFunc</code> function pointer and a <code>DelegateIdType</code>.</p>

<p><code>DelegateRemoteInvoker::Invoke()</code> calls <code>DelegateInvoke()</code> with the incoming data stream. Each function parameter is deserialized using <code>operator&lt;&lt;</code> and the <code>FreeFunc</code> target function is called.</p>

<p>The member function delegate variants are implemented similarly.</p>

<h3>Asynchronous Delegate Details</h3>

<p>Asynchronous delegates are also part of delegate library. The porting details for those features are covered within the <a href="https://www.codeproject.com/Articles/1160934/Asynchronous-Multicast-Delegates-in-Cplusplus">Asynchronous Multicast Delegates in C++</a> article.</p>

<h2>Source Code</h2>

<p>The attached source code contains the entire delegate library and numerous examples for Visual Studio&nbsp;and Eclipse.</p>

<h3>Visual Studio</h3>

<p>The Visual Studio/Win32&nbsp;examples have three separate projects:</p>

<ol>
	<li><strong>RemoteDelegate</strong> &ndash; many samples where the remote sender and receiver are executing within the same application.</li>
	<li><strong>RemoteDelegeteSend</strong> &ndash; a UDP remote delegate sending console application.</li>
	<li><strong>RemoteDelegateRecv</strong> &ndash; a UDP receiving console application that receives from <code>RemoteDelegateSend</code>.</li>
</ol>

<p>To run the JSON examples, you&rsquo;ll need to:</p>

<ol>
	<li>Clone the <code>RapidJSON</code> library into a <em>rapidjson</em> directory within your <em>RemoteDelegate</em> source directory.</li>
	<p><a href="https://github.com/Tencent/rapidjson/">https://github.com/Tencent/rapidjson/</a></p>
	<li>Define <strong>RAPID_JSON</strong> in the <strong>C/C++ &gt; Preprocessor &gt; Preprocessor Definitions</strong> within Visual Studio.</li>
</ol>

<h3>Eclipse</h3>

<p>The Eclipse/POSIX examples are imported into a workspace using <strong>File &gt; Import... &gt; Existing Projects into Workspace</strong>.</p>

<p>To run the JSON examples, you&rsquo;ll need to:</p>

<ol>
	<li>Clone the <code>RapidJSON</code> library into a <em>rapidjson</em> directory within your <em>RemoteDelegate</em> source directory.</li>
	<p><a href="https://github.com/Tencent/rapidjson/">https://github.com/Tencent/rapidjson/</a></p>
	<li>Define <strong>RAPID_JSON </strong>in <strong>Properties &gt; C/C++ General &gt; Paths and Symbols &gt; Symbols</strong> tab.</li>
</ol>

<h2>Conclusion</h2>

<p>I&rsquo;ve been using the C++ delegate library on a few different projects now. The ease at which functions and data can be moved between threads has really changed how I create multi-threaded applications.</p>

<p>The remote delegate enhancements extend library by allowing remote notification. The library eases application development by passing data between remote systems using a simple delegate mechanism.</p>

<h2>References</h2>

<ul>
	<li><a href="https://www.codeproject.com/Articles/1160934/Asynchronous-Multicast-Delegates-in-Cplusplus">Asynchronous Multicast Delegates in C++</a> - by David Lafreniere</li>
</ul>





