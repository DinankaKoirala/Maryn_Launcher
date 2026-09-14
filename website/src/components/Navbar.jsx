import { Link } from 'react-router-dom'
import { useState } from "react";

export default function Navbar() {
    const [menuOpen, setMenuOpen] = useState(false)

    return(
        <nav className="bg-zinc-900 border-b border-zinc-700 px-8 py-4 ">
            <div className="flex items-center justify-between">
            <Link to="/" className="text-white font-bold text-xl">
            Maryn
            </Link>
            <div className="hidden sm:flex gap-6">
                <Link to="/download" className="text-zinc-300 hover:text-white transition-colors">Download</Link>
                <Link to="/community" className="text-zinc-300 hover:text-white transition-colors">Community</Link>
                <Link to="/donate" className="text-zinc-300 hover:text-white transition-colors">Donate</Link>
            </div>
            
            <button className="sm:hidden  text-white text-2xl" onClick={() => setMenuOpen(!menuOpen)}>
                ☰
            </button>
            </div>

        {menuOpen && (
        <div className="sm:hidden flex flex-col gap-4 mt-4">
          <Link to="/download" className="text-zinc-300 hover:text-white">
            Download
          </Link>

          <Link to="/community" className="text-zinc-300 hover:text-white">
            Community
          </Link>

          <Link to="/donate" className="text-zinc-300 hover:text-white">
            Donate
          </Link>
        </div>
        )}
            
        </nav>
    )
}