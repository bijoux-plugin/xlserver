package bijoux.mockxlclient.xloper;

public class XLNil extends XLoper
{
	public static XLNil NIL = new XLNil();

	private XLNil()
	{
		super(xlTypeNil);
	}

	public String toString()
	{
		return "";
	}
}
