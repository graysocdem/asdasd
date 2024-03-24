using System.Diagnostics;
using System.Runtime.InteropServices;

public class Program
{
    [StructLayout(LayoutKind.Sequential)]
    public struct TransferPoint
    {
        public int x;
        public int y;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct TransferPointArray
    {
        public IntPtr arrayPtr;
        public nint length; // Change the type to IntPtr
    }

    //[DllImport("ashafhadfh.dll")]
    [DllImport(@"C:\Users\Martin\source\repos\Boost test\x64\Debug\Dll1.dll")]

    public static extern TransferPointArray get_voronoi_vertices();

    public static void Main(string[] args)
    {
        TransferPointArray transferPointsArray = get_voronoi_vertices();

        // Check if the pointer is null
        if (transferPointsArray.arrayPtr == IntPtr.Zero)
        {
            Console.WriteLine("Error: Pointer to transferPoints array is null.");
            Console.WriteLine(transferPointsArray.length);

            return;
        }

        // Get the length using nint
        nint length = transferPointsArray.length;

        // Calculate the size of the array
        int elementSize = Marshal.SizeOf<TransferPoint>();
        int byteSize = elementSize * (int)length;

        // Allocate memory for the byte array
        byte[] byteArray = new byte[byteSize];

        // Marshal the pointer to the byte array
        Marshal.Copy(transferPointsArray.arrayPtr, byteArray, 0, byteSize);

        //// Marshal the byte array to the TransferPoint array
        //TransferPoint[] transferPoints = new TransferPoint[length];
        //GCHandle handle = GCHandle.Alloc(byteArray, GCHandleType.Pinned);
        //Marshal.PtrToStructure(handle.AddrOfPinnedObject(), transferPoints);
        //handle.Free();

        TransferPoint[] transferPoints = new TransferPoint[length];
        int structSize = Marshal.SizeOf<TransferPoint>();
        for (int i = 0; i < length; i++)
        {
            IntPtr ptr = IntPtr.Add(transferPointsArray.arrayPtr, i * structSize);
            transferPoints[i] = Marshal.PtrToStructure<TransferPoint>(ptr);
        }

        Console.WriteLine(transferPoints[0].y);
    }
}

