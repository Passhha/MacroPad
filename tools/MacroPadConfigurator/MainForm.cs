using System.IO.Ports;

namespace MacroPadConfigurator;

public sealed class MainForm : Form
{
    private readonly ComboBox portBox = new() { DropDownStyle = ComboBoxStyle.DropDownList };
    private readonly ComboBox modeBox = new() { DropDownStyle = ComboBoxStyle.DropDownList };
    private readonly ComboBox actionBox = new() { DropDownStyle = ComboBoxStyle.DropDownList };
    private readonly ComboBox keyBox = new() { DropDownStyle = ComboBoxStyle.DropDownList };
    private readonly CheckBox controlBox = new() { Text = "Ctrl", AutoSize = true };
    private readonly CheckBox shiftBox = new() { Text = "Shift", AutoSize = true };
    private readonly CheckBox altBox = new() { Text = "Alt", AutoSize = true };
    private readonly CheckBox guiBox = new() { Text = "Win", AutoSize = true };
    private readonly Label selectedKeyLabel = new() { Text = "Select a key", AutoSize = true };
    private readonly Button applyButton = new() { Text = "Apply binding", Enabled = false, AutoSize = true };
    private readonly SerialPort serialPort = new() { BaudRate = 115200, NewLine = "\n" };
    private int selectedKey = -1;

    public MainForm()
    {
        Text = "MacroPad X Configurator";
        ClientSize = new Size(560, 480);
        FormBorderStyle = FormBorderStyle.FixedDialog;
        MaximizeBox = false;

        modeBox.Items.AddRange(["System", "Chrome", "Discord"]);
        modeBox.SelectedIndex = 0;
        actionBox.Items.AddRange(["Keyboard", "Media", "Disabled", "Settings screen"]);
        actionBox.SelectedIndex = 0;
        actionBox.SelectedIndexChanged += (_, _) => UpdateActionControls();

        AddKeyChoices();
        RefreshPorts();

        var refreshButton = new Button { Text = "Refresh", AutoSize = true };
        refreshButton.Click += (_, _) => RefreshPorts();
        var connectButton = new Button { Text = "Connect", AutoSize = true };
        connectButton.Click += (_, _) => ToggleConnection(connectButton);
        applyButton.Click += (_, _) => ApplyBinding();

        var connectionPanel = new FlowLayoutPanel { Dock = DockStyle.Top, AutoSize = true, Padding = new Padding(12) };
        connectionPanel.Controls.AddRange([new Label { Text = "COM port:", AutoSize = true }, portBox, refreshButton, connectButton,
            new Label { Text = "Mode:", AutoSize = true, Margin = new Padding(20, 6, 3, 3) }, modeBox]);

        var keyGrid = new TableLayoutPanel { ColumnCount = 5, RowCount = 4, Dock = DockStyle.Top, Padding = new Padding(12), AutoSize = true };
        for (var key = 0; key < 20; ++key)
        {
            var button = new Button { Text = (key + 1).ToString(), Width = 90, Height = 48, Tag = key };
            button.Click += SelectKey;
            keyGrid.Controls.Add(button, key % 5, key / 5);
        }

        var settingsPanel = new FlowLayoutPanel { Dock = DockStyle.Top, AutoSize = true, Padding = new Padding(12), FlowDirection = FlowDirection.TopDown };
        var modifiers = new FlowLayoutPanel { AutoSize = true };
        modifiers.Controls.AddRange([controlBox, shiftBox, altBox, guiBox]);
        settingsPanel.Controls.Add(selectedKeyLabel);
        settingsPanel.Controls.Add(actionBox);
        settingsPanel.Controls.Add(keyBox);
        settingsPanel.Controls.Add(modifiers);
        settingsPanel.Controls.Add(applyButton);

        Controls.Add(settingsPanel);
        Controls.Add(keyGrid);
        Controls.Add(connectionPanel);
        UpdateActionControls();
    }

    protected override void Dispose(bool disposing)
    {
        if (disposing)
        {
            serialPort.Dispose();
        }

        base.Dispose(disposing);
    }

    private void AddKeyChoices()
    {
        for (var code = 4; code <= 29; ++code)
        {
            keyBox.Items.Add(new HidChoice(((char)('A' + code - 4)).ToString(), code));
        }

        for (var code = 30; code <= 39; ++code)
        {
            keyBox.Items.Add(new HidChoice((code == 39 ? "0" : (code - 29).ToString()), code));
        }

        for (var code = 58; code <= 69; ++code)
        {
            keyBox.Items.Add(new HidChoice($"F{code - 57}", code));
        }

        for (var code = 104; code <= 115; ++code)
        {
            keyBox.Items.Add(new HidChoice($"F{code - 91}", code));
        }

        keyBox.Items.AddRange([
            new HidChoice("Enter", 40), new HidChoice("Escape", 41), new HidChoice("Space", 44),
            new HidChoice("Tab", 43), new HidChoice("Left", 80), new HidChoice("Right", 79),
            new HidChoice("Up", 82), new HidChoice("Down", 81)
        ]);
        keyBox.SelectedIndex = 0;
    }

    private void RefreshPorts()
    {
        var selectedPort = portBox.SelectedItem?.ToString();
        portBox.Items.Clear();
        portBox.Items.AddRange(SerialPort.GetPortNames().Order());

        if (selectedPort is not null && portBox.Items.Contains(selectedPort))
        {
            portBox.SelectedItem = selectedPort;
        }
        else if (portBox.Items.Count > 0)
        {
            portBox.SelectedIndex = 0;
        }
    }

    private void ToggleConnection(Button button)
    {
        try
        {
            if (serialPort.IsOpen)
            {
                serialPort.Close();
                button.Text = "Connect";
                return;
            }

            if (portBox.SelectedItem is null)
            {
                MessageBox.Show("Select the device COM port first.", Text, MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }

            serialPort.PortName = portBox.SelectedItem.ToString()!;
            serialPort.Open();
            button.Text = "Disconnect";
        }
        catch (Exception exception)
        {
            MessageBox.Show(exception.Message, Text, MessageBoxButtons.OK, MessageBoxIcon.Error);
        }
    }

    private void SelectKey(object? sender, EventArgs arguments)
    {
        if (sender is not Button { Tag: int key })
        {
            return;
        }

        selectedKey = key;
        selectedKeyLabel.Text = $"Selected key: {selectedKey + 1}";
        applyButton.Enabled = true;
    }

    private void UpdateActionControls()
    {
        var keyboardAction = actionBox.SelectedIndex == 0;
        var mediaAction = actionBox.SelectedIndex == 1;
        keyBox.Enabled = keyboardAction || mediaAction;
        controlBox.Enabled = keyboardAction;
        shiftBox.Enabled = keyboardAction;
        altBox.Enabled = keyboardAction;
        guiBox.Enabled = keyboardAction;

        if (mediaAction)
        {
            keyBox.Items.Clear();
            keyBox.Items.AddRange([
                new HidChoice("Mute", 226), new HidChoice("Volume up", 233), new HidChoice("Volume down", 234),
                new HidChoice("Play/Pause", 205), new HidChoice("Next track", 181), new HidChoice("Previous track", 182)
            ]);
            keyBox.SelectedIndex = 0;
        }
        else if (keyboardAction && keyBox.Items.Count < 20)
        {
            keyBox.Items.Clear();
            AddKeyChoices();
        }
    }

    private void ApplyBinding()
    {
        if (!serialPort.IsOpen || selectedKey < 0)
        {
            MessageBox.Show("Connect the MacroPad and select a key.", Text, MessageBoxButtons.OK, MessageBoxIcon.Information);
            return;
        }

        var type = actionBox.SelectedIndex switch { 0 => 1, 1 => 2, 2 => 0, _ => 3 };
        var modifiers = type == 1
            ? (controlBox.Checked ? 1 : 0) | (shiftBox.Checked ? 2 : 0) | (altBox.Checked ? 4 : 0) | (guiBox.Checked ? 8 : 0)
            : 0;
        var code = type is 0 or 3 ? 0 : ((HidChoice)keyBox.SelectedItem!).Code;

        try
        {
            serialPort.WriteLine($"MODE {modeBox.SelectedItem!.ToString()!.ToUpperInvariant()}");
            serialPort.WriteLine($"BIND {modeBox.SelectedIndex} {selectedKey} {type} {modifiers} {code}");
        }
        catch (Exception exception)
        {
            MessageBox.Show(exception.Message, Text, MessageBoxButtons.OK, MessageBoxIcon.Error);
        }
    }

    private sealed record HidChoice(string Name, int Code)
    {
        public override string ToString() => Name;
    }
}
