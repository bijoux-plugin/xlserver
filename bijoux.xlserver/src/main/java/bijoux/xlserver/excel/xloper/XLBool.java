package bijoux.xlserver.excel.xloper;

public class XLBool extends XLoper
{
	public static final XLBool TRUE  = new XLBool(true);
	public static final XLBool FALSE = new XLBool(false);

	public final boolean       bool;

	public XLBool(boolean bool)
	{
		super(xlTypeBool);
		this.bool = bool;
	}

	public String toString()
	{
		return Boolean.toString(bool);
	}
}
