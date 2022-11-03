#include "pont.h"
#include "folyoso.h"



void pont::move_in(folyoso sarok)
{
	if (y > 1)
	{
		if (x < 0)
		{
			y = 1;
		}
		else if (y > (1 + x * sarok.tgphi_out()))
		{
			y = 1 + x * sarok.tgphi_out();
		}
	}
	else if (y < 0)
	{
		if (x < sarok.x0_out())
		{
			y = 0;
		}
	}
	if (y < (x - sarok.x0_out()) * sarok.tgphi_out())
	{
		y = (x - sarok.x0_out()) * sarok.tgphi_out();
	}
}

bool pont::move_in_print(folyoso sarok) const
{

	if (y > 1)
	{
		if (x < 0)
		{
			//y = 1;
			return true;
		}
		else if (y > (1 + x * sarok.tgphi_out()))
		{
			//y = 1 + x * sarok.tgphi_out();
			return true;
		}
	}
	else if (y < 0)
	{
		if (x < sarok.x0_out())
		{
			//y = 0;
			return true;
		}
	}
	if (y < (x - sarok.x0_out()) * sarok.tgphi_out())
	{
		//y = sarok.x0_out() + x * sarok.tgphi_out();
		return true;
	}

	return false;
}

void pont::first_cut(folyoso sarok)
{
	if (y > 1)
	{
		y = 1;
	}
	else if (y < 0)
	{
		if (x < sarok.x0_out())
		{
			y = 0;
		}
	}
	if (y < (x - sarok.x0_out()) * sarok.tgphi_out())
	{
		y = (x - sarok.x0_out()) * sarok.tgphi_out();
	}
}

void pont::last_cut(folyoso sarok)
{
	if (y > 1)
	{
		if (x < 0)
		{
			y = 1;
		}
		else if (y > (1 + x * sarok.tgphi_out()))
		{
			y = 1 + x * sarok.tgphi_out();
		}
	}
	else if (y < 0)
	{
		if (x < sarok.x0_out())
		{
			y = 0;
		}
	}
	if (y < (x - sarok.x0_out()) * sarok.tgphi_out())
	{
		y = (x - sarok.x0_out()) * sarok.tgphi_out();
	}

	if (y > (1 + x * sarok.tgphi_out()))
	{
		y = (1 + x * sarok.tgphi_out());
	}
	if (y < 0)
	{
		y = 0;
	}
	if (-1 / sarok.tgphi_out() > x)
	{
		x = -1 / sarok.tgphi_out();
	}

}
