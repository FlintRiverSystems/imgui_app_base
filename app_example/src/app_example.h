
class app_example : public imgui_app_base
{
public:
	app_example() {};
	virtual ~app_example() {};

protected:
	void _render_ui() override;

private:

	bool _show_demo_window = true;
	bool _show_another_window = false;
};
