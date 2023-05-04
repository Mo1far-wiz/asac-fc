import Tab from "./Tab";

function Tabs() {
    let configs = [
        {name: "Gyro"},
        {name: "Pid"},
    ]
    return (
        <div>
            <ul className="nav nav-tabs" id="myTab" role="tablist">
                {configs.map((c, index) =>
                    <li className="nav-item" role="presentation">
                        <button className={'nav-link' + (index == 0 ? ' active' : '')}
                                id={`${c.name}-tab`} data-bs-toggle="tab" data-bs-target={`#${c.name}`}
                                type="button" role="tab" aria-controls={`${c.name}`} aria-selected="true">
                            { c.name }
                        </button>
                    </li>
                )}
            </ul>

            <div className="tab-content" id="myTabContent">
                {configs.map((c, index) =>
                    <div className={'tab-pane fade show' + (index == 0 ? ' active' : '')} id={`${c.name}`}
                         role="tabpanel" aria-labelledby={`${c.name}-tab`}>
                        <Tab config={c}/>
                    </div>
                )}
            </div>
        </div>
    );
}

export default Tabs;