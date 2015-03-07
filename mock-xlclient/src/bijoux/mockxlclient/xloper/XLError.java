package bijoux.mockxlclient.xloper;

public class XLError extends XLoper
{
	public static final XLError NULL  = new XLError(0);
	public static final XLError DIV0  = new XLError(7);
	public static final XLError VALUE = new XLError(15);
	public static final XLError REF   = new XLError(23);
	public static final XLError NAME  = new XLError(29);
	public static final XLError NUM   = new XLError(36);
	public static final XLError NA    = new XLError(42);

	public final int            err;

	public XLError(int err)
	{
		super(xlTypeErr);
		this.err = err;
	}

	public String toString()
	{
		return Integer.toString(err);
	}
}
