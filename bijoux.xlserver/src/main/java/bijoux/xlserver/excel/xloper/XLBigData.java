package bijoux.xlserver.excel.xloper;

public class XLBigData extends XLoper
{
	public XLBigData(byte[] data, int length)
	{
		super(XLoper.xlTypeBigData);
		this.data = data;
		this.length = length;
	}
	
	public final byte[] data;
	public final int length;
}