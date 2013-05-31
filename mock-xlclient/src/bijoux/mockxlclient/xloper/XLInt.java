package bijoux.mockxlclient.xloper;

public class XLInt extends XLoper
{
	public final int w;

	public XLInt(int w)
	{
		super(xlTypeInt);
		this.w = w;
	}

	public String toString()
	{
		return Integer.toString(w);
	}
}
