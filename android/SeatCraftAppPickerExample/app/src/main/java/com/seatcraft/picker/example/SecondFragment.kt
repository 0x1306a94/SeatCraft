package com.seatcraft.picker.example

import android.content.Context
import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import com.seatcraft.picker.example.databinding.FragmentSecondBinding

/**
 * A simple [Fragment] subclass as the second destination in the navigation.
 */
class SecondFragment : Fragment() {

    private var _binding: FragmentSecondBinding? = null

    // This property is only valid between onCreateView and
    // onDestroyView.
    private val binding get() = _binding!!

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View {

        _binding = FragmentSecondBinding.inflate(inflater, container, false)
        return binding.root

    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        binding.pickerView.setAreaMapSvgPath("asset://svg/performbg_2.svg")
    }

    override fun onPause() {
        super.onPause()
        binding.pickerView.onPause()
    }

    override fun onResume() {
        super.onResume()
        binding.pickerView.onResume()
    }

    override fun onDestroy() {
        super.onDestroy()
        binding.pickerView.onDestroy()
    }

    private fun readAssetFileToByteArray(context: Context, fileName: String): ByteArray? {
        return try {
            context.assets.open(fileName).use { inputStream ->
                inputStream.readBytes()
            }
        } catch (e: Exception) {
            e.printStackTrace()
            null
        }
    }
}