package com.test.cmaketest

import android.app.ActivityManager
import android.content.Context
import android.os.Bundle
import android.util.Log
import androidx.appcompat.app.AppCompatActivity
import com.test.cmaketest.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity(), ConnectionListener {

    private lateinit var binding: ActivityMainBinding

    private var token: Long = 0

    private var nativePtr: Long = 0

    private var mSyncNetWorkState: SyncNetWorkState? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        // Example of a call to a native method
        binding.sampleText.text = stringFromJNI()
        nativePtr = createListener()
        mSyncNetWorkState = SyncNetWorkState(nativePtr)
        mSyncNetWorkState?.addConnectionChangeListener(this)

        val capabilities: Capabilities = AndroidCapabilities()
        val foreverNotifier: ForeverNotifier = AndroidForeverNotifier(capabilities)
        createJavaBindingContext(foreverNotifier)

        val activityManager = getSystemService(Context.ACTIVITY_SERVICE)
        val configurationInfo = (activityManager as ActivityManager).deviceConfigurationInfo
        val supportsEs2 = configurationInfo.reqGlEsVersion >= 0x20000
        Log.e("reqGlEsVersion", configurationInfo.reqGlEsVersion.toString())
    }

    companion object {
        // Used to load the 'cmaketest' library on application startup.
        init {
            System.loadLibrary("cmaketest")
        }
    }

    override fun onResume() {
        super.onResume()
        execListener(nativePtr)
    }

    override fun onStop() {
        super.onStop()
        execListener(nativePtr)
    }

    override fun onDestroy() {
        super.onDestroy()
        destroyJavaBindingContext()
        mSyncNetWorkState?.removeConnectionChangeListener(this)
    }

    /**
     * A native method that is implemented by the 'cmaketest' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String

    external fun createListener(): Long
    external fun execListener(ptr: Long)
    fun onChange(oldState: Long, newState: Long) {
        Log.e("ConnectionState", "oldState:$oldState newState:$newState")
    }

    external fun createJavaBindingContext(notifier: ForeverNotifier)

    external fun destroyJavaBindingContext()

    override fun onChange(oldState: ConnectionState?, newState: ConnectionState?) {
        Log.e("ConnectionState", "oldState:$oldState newState:$newState")
    }
}