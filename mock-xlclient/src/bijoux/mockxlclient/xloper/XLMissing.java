package bijoux.mockxlclient.xloper;

public class XLMissing extends XLoper
{
	public static final XLMissing MISSING = new XLMissing();

	XLMissing()
	{
		super(xlTypeMissing);
	}

	public String toString()
	{
		return "";
	}
}
