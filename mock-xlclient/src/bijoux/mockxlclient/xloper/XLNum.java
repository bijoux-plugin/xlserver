package bijoux.mockxlclient.xloper;

public final class XLNum extends XLoper
{
	public final double num;

	public XLNum(double num)
	{
		super(xlTypeNum);
		this.num = num;
	}

	public String toString()
	{
		return Double.toString(num);
	}
}